#pragma once

#include "openGLRenderer.h"
#include "../glframework/core.h"
#include "../glframework/object.h"
#include "../glframework/scene.h"
#include "../glframework/geometry.h"
#include "../glframework/texture.h"
#include "../glframework/mesh.h"
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
	//��ȼ�����
	void enableDepthTest(Object* root);
	void disableDepthTest(Object* root);
	void depthFunc(Object* root, GLenum depthFunc);
	void enableDepthWrite(Object* root);
	void disableDepthWrite(Object* root);

	//polygonOffset���
	void enablePolygonOffsetFill(Object* root);
	void disablePolygonOffsetFill(Object* root);
	void enablePolygonOffsetLine(Object* root);
	void disablePolygonOffsetLine(Object* root);
	void polygonOffset(Object* root, float factor, float unit);

	//stencil���
	void enableStencilTest(Object* root);
	void disableStencilTest(Object* root);
	void stencilOp(Object* root, GLenum fail, GLenum zfail, GLenum zpass);
	void stencilMask(Object* root, GLuint mask);
	void stencilFunc(Object* root, GLenum func, GLint ref, GLuint mask);

	//��ɫ���
	void enableBlend(Object* root);
	void disableBlend(Object* root);
	void blendFunc(Object* root, GLenum sfactor, GLenum dfactor);
	void blendFunci(Object* root, GLuint buf, GLenum src, GLenum dst);
	void blendFuncSeparate(Object* root, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
	void blendEquation(Object* root, GLenum mode);

	//���޳�
	void enableCullFace(Object* root);
	void disableCullFace(Object* root);
	void setFrontFace(Object* root, GLenum face);
	void cullFace(Object* root, GLenum face);

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