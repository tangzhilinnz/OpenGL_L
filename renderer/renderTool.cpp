#include "renderTool.h"
#include "../wrapper/checkError.h"

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
	AttachmentGL::destroyAllInstances();
	FboGL::destroyAllInstances();
	Texture::clearCache();
}

void RenderTool::setOpcity(Object* root, float opacity)
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


std::vector<Mesh*> RenderTool::extractMesh(Object* root)
{
	std::vector<Mesh*> meshVec;

	RenderTool::objectIterator(root,
		[&meshVec](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			meshVec.push_back(mesh);
		});

	return meshVec;
}

void RenderTool::setMaterial(Object* root, Material* mat)
{
	RenderTool::objectIterator(root,
		[mat](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->setMaterial(mat);
		});
}

///////////////////////////////////////////////////////////////////////////////
void RenderTool::enableDepthTest(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enableDepthTest();
		});
}

void RenderTool::disableDepthTest(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->disableDepthTest();
		});
}

void RenderTool::depthFunc(Object* root, GLenum depthFunc)
{
	RenderTool::objectIterator(root,
		[depthFunc](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->depthFunc(depthFunc);
		});
}

void RenderTool::enableDepthWrite(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enableDepthWrite();
		});
}

void RenderTool::disableDepthWrite(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->disableDepthWrite();
		});
}

//polygonOffset相关
void RenderTool::enablePolygonOffsetFill(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enablePolygonOffsetFill();
		});
}

void RenderTool::disablePolygonOffsetFill(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->disablePolygonOffsetFill();
		});
}

void RenderTool::enablePolygonOffsetLine(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enablePolygonOffsetLine();
		});
}

void RenderTool::disablePolygonOffsetLine(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->disablePolygonOffsetLine();
		});
}

void RenderTool::polygonOffset(Object* root, float factor, float unit)
{
	RenderTool::objectIterator(root,
		[factor, unit](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->polygonOffset(factor, unit);
		});
}

//stencil相关
void RenderTool::enableStencilTest(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enableStencilTest();
		});
}

void RenderTool::disableStencilTest(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->disableStencilTest();
		});
}

void RenderTool::stencilOp(Object* root, GLenum fail, GLenum zfail, GLenum zpass)
{
	RenderTool::objectIterator(root,
		[fail, zfail, zpass](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->stencilOp(fail, zfail, zpass);
		});
}

void RenderTool::stencilMask(Object* root, GLuint mask)
{
	RenderTool::objectIterator(root,
		[mask](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->stencilMask(mask);
		});
}

void RenderTool::stencilFunc(Object* root, GLenum func, GLint ref, GLuint mask)
{
	RenderTool::objectIterator(root,
		[func, ref, mask](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->stencilFunc(func, ref, mask);
		});
}

//颜色混合
void RenderTool::enableBlend(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enableBlend();
		});
}

void RenderTool::disableBlend(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->disableBlend();
		});
}

void RenderTool::blendFunc(Object* root, GLenum sfactor, GLenum dfactor)
{
	RenderTool::objectIterator(root,
		[sfactor, dfactor](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->blendFunc(sfactor, dfactor);
		});
}

void RenderTool::blendFunci(Object* root, GLuint buf, GLenum src, GLenum dst)
{
	RenderTool::objectIterator(root,
		[buf, src, dst](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->blendFunci(buf, src, dst);
		});
}

void RenderTool::blendFuncSeparate(Object* root, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
	RenderTool::objectIterator(root,
		[sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->blendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
		});
}

void RenderTool::blendEquation(Object* root, GLenum mode)
{
	RenderTool::objectIterator(root,
		[mode](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->blendEquation(mode);
		});
}

//面剔除
void RenderTool::enableCullFace(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->enableCullFace();
		});
}

void RenderTool::disableCullFace(Object* root)
{
	RenderTool::objectIterator(root,
		[](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->disableCullFace();
		});
}

void RenderTool::setFrontFace(Object* root, GLenum face)
{
	RenderTool::objectIterator(root,
		[face](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->setFrontFace(face);
		});
}

void RenderTool::cullFace(Object* root, GLenum face)
{
	RenderTool::objectIterator(root,
		[face](Object* obj) {
			Mesh* mesh = (Mesh*)obj;
			mesh->cullFace(face);
		});
}
///////////////////////////////////////////////////////////////////////////////