#include "mesh.h"

State::State()
{
	this->enableDepthTest();
	this->enableDepthWrite();
	this->depthFunc(GL_LESS);
	this->disablePolygonOffsetFill();
	this->disablePolygonOffsetLine();
	this->disableStencilTest();
	this->disableBlend();
}

State::~State()
{
	printf("---- ~State(Mesh) ----\n");
}

//深度检测相关
void State::enableDepthTest()
{
	stateFuncs[StateType::DEPTH_TEST] =
		[]() { GL_CALL(glEnable(GL_DEPTH_TEST)); };
	stateFlags[StateType::DEPTH_TEST] = true;
}

void State::disableDepthTest()
{
	stateFuncs[StateType::DEPTH_TEST] =
		[]() { GL_CALL(glDisable(GL_DEPTH_TEST)); };
	stateFuncs.erase(StateType::DEPTH_FUNC);
	stateFlags[StateType::DEPTH_TEST] = false;
}

void State::depthFunc(GLenum depthFunc)
{
	stateFuncs[StateType::DEPTH_FUNC] =
		[depthFunc]() { GL_CALL(glDepthFunc(depthFunc)); };
}

void State::enableDepthWrite()
{
	stateFuncs[StateType::DEPTH_WRITE] =
		[]() { GL_CALL(glDepthMask(GL_TRUE)); };
	stateFlags[StateType::DEPTH_WRITE] = true;
}

void State::disableDepthWrite()
{
	stateFuncs[StateType::DEPTH_WRITE] =
		[]() { GL_CALL(glDepthMask(GL_FALSE)); };
	stateFlags[StateType::DEPTH_WRITE] = false;
}

//polygonOffset相关
void State::enablePolygonOffsetFill()
{
	stateFuncs[StateType::POLYGON_OFFSET_FILL] =
		[]() { GL_CALL(glEnable(GL_POLYGON_OFFSET_FILL)); };
	stateFlags[StateType::POLYGON_OFFSET_FILL] = true;
}

void State::disablePolygonOffsetFill()
{
	stateFuncs[StateType::POLYGON_OFFSET_FILL] =
		[]() { GL_CALL(glDisable(GL_POLYGON_OFFSET_FILL)); };
	stateFuncs.erase(StateType::POLYGON_OFFSET);
	stateFlags[StateType::POLYGON_OFFSET_FILL] = false;
}

void State::enablePolygonOffsetLine()
{
	stateFuncs[StateType::POLYGON_OFFSET_LINE] =
		[]() { GL_CALL(glEnable(GL_POLYGON_OFFSET_LINE)); };
	stateFlags[StateType::POLYGON_OFFSET_LINE] = true;
}

void State::disablePolygonOffsetLine()
{
	stateFuncs[StateType::POLYGON_OFFSET_LINE] =
		[]() { GL_CALL(glDisable(GL_POLYGON_OFFSET_LINE)); };
	stateFuncs.erase(StateType::POLYGON_OFFSET);
	stateFlags[StateType::POLYGON_OFFSET_LINE] = false;
}

void State::polygonOffset(float factor, float unit)
{
	stateFuncs[StateType::POLYGON_OFFSET] =
		[factor, unit]() { GL_CALL(glPolygonOffset(factor, unit)); };
}

//stencil相关
void State::enableStencilTest()
{
	stateFuncs[StateType::STENCIL_TEST] =
		[]() { GL_CALL(glEnable(GL_STENCIL_TEST)); };
	stateFlags[StateType::STENCIL_TEST] = true;
}

void State::disableStencilTest()
{
	stateFuncs[StateType::STENCIL_TEST] =
		[]() { GL_CALL(glDisable(GL_STENCIL_TEST)); };
	stateFuncs.erase(StateType::STENCIL_OP);
	stateFuncs.erase(StateType::STENCIL_MASK);
	stateFuncs.erase(StateType::STENCIL_FUNC);
	stateFlags[StateType::STENCIL_TEST] = false;
}

void State::stencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	stateFuncs[StateType::STENCIL_OP] =
		[fail, zfail, zpass]() { GL_CALL(glStencilOp(fail, zfail, zpass)); };
}

void State::stencilMask(GLuint mask)
{
	stateFuncs[StateType::STENCIL_MASK] =
		[mask]() { GL_CALL(glStencilMask(mask)); };
}

void State::stencilFunc(GLenum func, GLint ref, GLuint mask)
{
	stateFuncs[StateType::STENCIL_FUNC] =
		[func, ref, mask]() { GL_CALL(glStencilFunc(func, ref, mask)); };
}


void State::enableBlend()
{
	stateFuncs[StateType::BLEND] =
		[]() { GL_CALL(glEnable(GL_BLEND)); };
	stateFlags[StateType::BLEND] = true;
}

void State::disableBlend()
{
	stateFuncs[StateType::BLEND] =
		[]() { GL_CALL(glDisable(GL_BLEND)); };
	stateFuncs.erase(StateType::BLEND_FUNC);
	stateFuncs.erase(StateType::BLEND_FUNC_SEP);
	stateFuncs.erase(StateType::BLEND_EQU);
	stateFlags[StateType::BLEND] = false;
}

void State::blendFunc(GLenum sfactor, GLenum dfactor)
{
	stateFuncs[StateType::BLEND_FUNC] =
		[sfactor, dfactor]() { GL_CALL(glBlendFunc(sfactor, dfactor)); };
}

void State::blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
	stateFuncs[StateType::BLEND_FUNC_SEP] =
		[sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha]()
		{ GL_CALL(glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha)); };
}

void State::blendEquation(GLenum mode)
{
	stateFuncs[StateType::BLEND_EQU] =
		[mode]() { GL_CALL(glBlendEquation(mode)); };
}

void State::applyState() const
{
	for (const auto& pair : stateFuncs)
	{
		// Calling the function stored in the map
		pair.second();
	}
}

bool State::getStateFlag(StateType stateType)
{
	return stateFlags[stateType];
}



// ================================= Mesh ====================================
Mesh* Mesh::createObj()
{
    Mesh* mesh = new Mesh;
    bookmark.push_back(mesh);
    return mesh;
}

Mesh* Mesh::createObj(Geometry* geometry, Material* material)
{
    Mesh* mesh = new Mesh(geometry, material);
    bookmark.push_back(mesh);
    return mesh;
}

Mesh::Mesh(Geometry* geometry, Material* material)
    : mGeometry(geometry),
    mMaterial(material)
{
    mType = ObjectType::Mesh;
}

Mesh::Mesh()
    : mGeometry(nullptr),
    mMaterial(nullptr)
{
    mType = ObjectType::Mesh;
}

Mesh::~Mesh()
{
    printf("---- ~Mesh ----\n");
}