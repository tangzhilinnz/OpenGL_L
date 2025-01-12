#include "renderTool.h"
#include "../wrapper/checkError.h"

std::vector<Mesh*>	RenderTool::mOpaqueObjects;
std::vector<Mesh*>	RenderTool::mTransparentObjects;

// Perform DFS on a scene tree using iterative version
void RenderTool::objectRender(Object* root, std::function<void(Object* obj)> method)
{
	RenderTool::objectIterator(root, method);
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


void RenderTool::extractMesh(Object* root, std::vector<Mesh*>& meshVec)
{
	RenderTool::objectIterator(root,
		[&meshVec](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			meshVec.push_back(mesh);
		});
}

void RenderTool::separateMesh(Object* root)
{
	//清空两个队列
	RenderTool::mOpaqueObjects.clear();
	RenderTool::mTransparentObjects.clear();

	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			if (mesh->getStateFlag(StateType::BLEND))
			{
				RenderTool::mTransparentObjects.push_back(mesh);
			}
			else
			{
				RenderTool::mOpaqueObjects.push_back(mesh);
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