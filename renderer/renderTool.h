#pragma once

#include "openGLRenderer.h"
#include "../glframework/core.h"
#include "../glframework/object.h"
#include "../glframework/scene.h"
#include "../glframework/geometry.h"
#include "../glframework/texture.h"
#include "../glframework/mesh.h"
#include "../glframework/shader.h"
#include "../glframework/framebuffer.h"
#include "../glframework/material/material.h"
#include "../application/camera/camera.h"

#include <assert.h>
#include <stack>
#include <functional>
#include <algorithm>

class RenderTool
{
public:
	// Perform DFS on a scene tree using iterative version
	static void objectRender(Object* root, std::function<void(Object* obj)> method);
	static void sceneClear();
	static void setOpcity(Object* root, float opacity = 1.0f);
	static void setMaterial(Object* root, Material* mat);
	static std::vector<Mesh*> extractMesh(Object* root);

public:
//======================== status setting for model ============================
	//深度检测相关
	static void enableDepthTest(Object* root);
	static void disableDepthTest(Object* root);
	static void depthFunc(Object* root, GLenum depthFunc);
	static void enableDepthWrite(Object* root);
	static void disableDepthWrite(Object* root);

	//polygonOffset相关
	static void enablePolygonOffsetFill(Object* root);
	static void disablePolygonOffsetFill(Object* root);
	static void enablePolygonOffsetLine(Object* root);
	static void disablePolygonOffsetLine(Object* root);
	static void polygonOffset(Object* root, float factor, float unit);

	//stencil相关
	static void enableStencilTest(Object* root);
	static void disableStencilTest(Object* root);
	static void stencilOp(Object* root, GLenum fail, GLenum zfail, GLenum zpass);
	static void stencilMask(Object* root, GLuint mask);
	static void stencilFunc(Object* root, GLenum func, GLint ref, GLuint mask);

	//颜色混合
	static void enableBlend(Object* root);
	static void disableBlend(Object* root);
	static void blendFunc(Object* root, GLenum sfactor, GLenum dfactor);
	static void blendFunci(Object* root, GLuint buf, GLenum src, GLenum dst);
	static void blendFuncSeparate(Object* root, GLenum sfactorRGB, GLenum dfactorRGB,
		                          GLenum sfactorAlpha, GLenum dfactorAlpha);
	static void blendEquation(Object* root, GLenum mode);

	//面剔除
	static void enableCullFace(Object* root);
	static void disableCullFace(Object* root);
	static void setFrontFace(Object* root, GLenum face);
	static void cullFace(Object* root, GLenum face);

//==============================================================================

private:
	static void objectIterator(Object* root, std::function<void(Object* obj)> func)
	{
		if (!root)
		{
			return;
		}

		std::stack<Object*> sk;
		sk.push(root);

		while (!sk.empty())
		{
			Object* object = sk.top();
			sk.pop();

			// Process the current object
			if (object->getType() == ObjectType::Mesh)
			{
				func(object);
			}

			// Push children onto the stack in reverse order to maintain
			// left-to-right traversal
			auto& children = object->getChildren();

			for (auto it = children.crbegin(); it != children.crend(); it++)
			{
				sk.push(*it);
			}
		}
	}
};