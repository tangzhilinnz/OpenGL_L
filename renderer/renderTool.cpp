#include "renderTool.h"

std::vector<Mesh*>	RenderTool::mOpacityObjects;
std::vector<Mesh*>	RenderTool::mTransparentObjects;

// Perform DFS on a scene tree using iterative version
void RenderTool::objectRender(Object* root, OpenGLRenderer* rdr)
{
	RenderTool::objectIterator(root, 
		[rdr](Object* obj) { rdr->meshRendering(obj);
		});
}

void RenderTool::objectSortedRender(Object* root, OpenGLRenderer* rdr)
{
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