#pragma once

#include"object.h"
#include "geometry.h"
#include "material/material.h"
#include "../wrapper/checkError.h"

#include <vector>
#include <unordered_map>
#include <functional>

enum class StateType
{
	DEPTH_TEST,
	DEPTH_FUNC,
	DEPTH_WRITE,
	POLYGON_OFFSET_FILL,
	POLYGON_OFFSET_LINE,
	POLYGON_OFFSET,
	STENCIL_TEST,
	STENCIL_OP,
	STENCIL_MASK,
	STENCIL_FUNC,
	BLEND,
	BLEND_FUNC,
	BLEND_FUNC_SEP,
	BLEND_EQU,
	CULL,
	FRONT_FACE,
	CULL_FACE
};

class State
{
public:
	State();
	State(const State&) = default;
	State& operator=(const State&) = default;
	~State();

public:
	//深度检测相关
	void enableDepthTest();
	void disableDepthTest();
	void depthFunc(GLenum depthFunc);
	void enableDepthWrite();
	void disableDepthWrite();

	//polygonOffset相关
	void enablePolygonOffsetFill();
	void disablePolygonOffsetFill();
	void enablePolygonOffsetLine();
	void disablePolygonOffsetLine();
	void polygonOffset(float factor, float unit);

	//stencil相关
	void enableStencilTest();
	void disableStencilTest();
	void stencilOp(GLenum fail, GLenum zfail, GLenum zpass);
	void stencilMask(GLuint mask);
	void stencilFunc(GLenum func, GLint ref, GLuint mask);

	//颜色混合
	void enableBlend();
	void disableBlend();
	void blendFunc(GLenum sfactor, GLenum dfactor);
	void blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
	void blendEquation(GLenum mode);

	//面剔除
	void enableCullFace();
	void disableCullFace();
	void setFrontFace(GLenum face);
	void cullFace(GLenum face);

	void applyState() const;

	bool getStateFlag(StateType stateType);

private:
	// Map to store the state and its enabled/disabled status
	std::unordered_map<StateType, std::function<void()>> stateFuncs;
	std::unordered_map<StateType, bool> stateFlags;
};

class Mesh : public Object
{
public:
	static Mesh* createObj();
	static Mesh* createObj(Geometry* geometry, Material* material);

	// Constructor using smart pointers
	Mesh(Geometry* geometry, Material* material);
	Mesh();
	Mesh(const Mesh&) = default;
	Mesh& operator=(const Mesh&) = default;
	~Mesh();

	// Getter for mGeometry
	inline Geometry* getGeometry() const
	{
		return mGeometry;
	}

	// Getter for mMaterial
	inline Material* getMaterial() const
	{
		return mMaterial;
	}

	inline void setGeometry(Geometry* geometry)
	{
		mGeometry = geometry;
	}

	inline void setMaterial(Material* material)
	{
		mMaterial = material;
	}

private:
	State mGLState;

private:
	// Mesh utilizes Geometry and Material without taking responsibility for
	// their memory management or initialization.
	Geometry* mGeometry{ nullptr };
	Material* mMaterial{ nullptr };

public:
	//深度检测相关
	void enableDepthTest()
	{
		mGLState.enableDepthTest();
	}
	void disableDepthTest()
	{
		mGLState.disableDepthTest();
	}
	void depthFunc(GLenum depthFunc)
	{
		mGLState.depthFunc(depthFunc);
	}
	void enableDepthWrite()
	{
		mGLState.enableDepthWrite();
	}
	void disableDepthWrite()
	{
		mGLState.disableDepthWrite();
	}

	//polygonOffset相关
	void enablePolygonOffsetFill()
	{
		mGLState.enablePolygonOffsetFill();
	}
	void disablePolygonOffsetFill()
	{
		mGLState.disablePolygonOffsetFill();
	}
	void enablePolygonOffsetLine()
	{
		mGLState.enablePolygonOffsetLine();
	}
	void disablePolygonOffsetLine()
	{
		mGLState.disablePolygonOffsetLine();
	}
	void polygonOffset(float factor, float unit)
	{
		mGLState.polygonOffset(factor, unit);
	}

	//stencil相关
	void enableStencilTest()
	{
		mGLState.enableStencilTest();
	}
	void disableStencilTest()
	{
		mGLState.disableStencilTest();
	}
	void stencilOp(GLenum fail, GLenum zfail, GLenum zpass)
	{
		mGLState.stencilOp(fail, zfail, zpass);
	}
	void stencilMask(GLuint mask)
	{
		mGLState.stencilMask(mask);
	}
	void stencilFunc(GLenum func, GLint ref, GLuint mask)
	{
		mGLState.stencilFunc(func, ref, mask);
	}

	//颜色混合
	void enableBlend()
	{
		mGLState.enableBlend();
	}
	void disableBlend()
	{
		mGLState.disableBlend();
	}
	void blendFunc(GLenum sfactor, GLenum dfactor)
	{
		mGLState.blendFunc(sfactor, dfactor);
	}
	void blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
	{
		mGLState.blendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
	}
	void blendEquation(GLenum mode)
	{
		mGLState.blendEquation(mode);
	}

	//面剔除
	void enableCullFace()
	{
		mGLState.enableCullFace();
	}
	void disableCullFace()
	{
		mGLState.disableCullFace();
	}
	void setFrontFace(GLenum face)
	{
		mGLState.setFrontFace(face);
	}
	void cullFace(GLenum face)
	{
		mGLState.cullFace(face);
	}


	void applyState() const
	{
		mGLState.applyState();
	}

	bool getStateFlag(StateType stateType)
	{
		return mGLState.getStateFlag(stateType);
	}
};