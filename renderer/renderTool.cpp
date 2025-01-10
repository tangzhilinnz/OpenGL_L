#include "renderTool.h"
#include "../wrapper/checkError.h"

std::vector<Mesh*>	RenderTool::mOpacityObjects;
std::vector<Mesh*>	RenderTool::mTransparentObjects;

// Perform DFS on a scene tree using iterative version
void RenderTool::objectRender(Object* root, OpenGLRenderer* rdr, GLuint fbo)
{
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

	//设置当前帧绘制的时候，opengl的必要状态机参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	GL_CALL(glDepthMask(GL_TRUE));

	GL_CALL(glDisable(GL_POLYGON_OFFSET_FILL));
	GL_CALL(glDisable(GL_POLYGON_OFFSET_LINE));

	//开启测试、设置基本写入状态，打开模板测试写入
	GL_CALL(glEnable(GL_STENCIL_TEST));
	GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
	GL_CALL(glStencilMask(0xFF));//保证了模板缓冲可以被清理

	//默认颜色混合
	GL_CALL(glDisable(GL_BLEND));

	//清理画布 
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	RenderTool::objectIterator(root, 
		[rdr](Object* obj) { rdr->meshRendering(obj);
		});
}

void RenderTool::objectSortedRender(Object* root, OpenGLRenderer* rdr, GLuint fbo)
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

	//设置当前帧绘制的时候，opengl的必要状态机参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	GL_CALL(glDepthMask(GL_TRUE));

	GL_CALL(glDisable(GL_POLYGON_OFFSET_FILL));
	GL_CALL(glDisable(GL_POLYGON_OFFSET_LINE));

	//开启测试、设置基本写入状态，打开模板测试写入
	GL_CALL(glEnable(GL_STENCIL_TEST));
	GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
	GL_CALL(glStencilMask(0xFF));//保证了模板缓冲可以被清理

	//默认颜色混合
	GL_CALL(glDisable(GL_BLEND));

	//清理画布 
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	//清空两个队列
	RenderTool::mOpacityObjects.clear();
	RenderTool::mTransparentObjects.clear();

	RenderTool::separateMesh(root);

	std::sort(
		RenderTool::mTransparentObjects.begin(),
		RenderTool::mTransparentObjects.end(),
		[rdr](const Mesh* a, const Mesh* b) {

			const Camera* cam = rdr->getCamera();

			auto viewMatrix = cam->GetViewMatrix();

			//1 计算a的相机系的Z
			auto modelMatrixA = a->getModelMatrix();
			auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionA = viewMatrix * worldPositionA;

			//2 计算b的相机系的Z
			auto modelMatrixB = b->getModelMatrix();
			auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionB = viewMatrix * worldPositionB;

			return cameraPositionA.z < cameraPositionB.z;
		}
	);

	//3 渲染两个队列
	for (size_t i = 0; i < RenderTool::mOpacityObjects.size(); i++)
	{
		rdr->meshRendering(mOpacityObjects[i]);
	}

	for (size_t i = 0; i < RenderTool::mTransparentObjects.size(); i++)
	{
		rdr->meshRendering(mTransparentObjects[i]);
	}
}

void RenderTool::sceneClear()
{
	Object::destroyAllInstances();
	Material::destroyAllInstances();
	Geometry::destroyAllInstances();
	Framebuffer::destroyAllInstances();
	Texture::clearCache();
}

void RenderTool::enableModelBlend(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enableBlend();
			mesh->blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			mesh->disableDepthWrite();
		});
}

void RenderTool::disableModelBlend(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enableDepthWrite();
			mesh->disableBlend();
		});
}

void RenderTool::setModelOpcity(Object* root, float opacity)
{
	if (opacity < 0 || opacity > 1)
	{
		return;
	}

	RenderTool::objectIterator(root,
		[opacity](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			Material* mat = mesh->getMaterial();
			mat->setOpacity(opacity);
		});
}

void RenderTool::separateMesh(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			if (mesh->getStateFlag(StateType::BLEND))
			{
				RenderTool::mTransparentObjects.push_back(mesh);
			}
			else
			{
				RenderTool::mOpacityObjects.push_back(mesh);
			}
		});
}

void RenderTool::setModelUniformMaterial(Object* root, Material* mat)
{
	RenderTool::objectIterator(root,
		[mat](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->setMaterial(mat);
		});
}