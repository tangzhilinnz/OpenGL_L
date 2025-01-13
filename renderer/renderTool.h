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
	static void enableModelBlend(Object* root);
	static void setModelOpcity(Object* root, float opacity = 1.0f);
	static void disableModelBlend(Object* root);

	static void setModelUniformMaterial(Object* root, Material* mat);

	static std::vector<Mesh*> extractMesh(Object* root);

	static void separateMesh(Object* root);

	static const std::vector<Mesh*>& getOpaqueObjects() { return mOpaqueObjects; }
	static const std::vector<Mesh*>& getTransparentObjects() { return mTransparentObjects; }

private:
	static std::vector<Mesh*>	mOpaqueObjects;
	static std::vector<Mesh*>	mTransparentObjects;

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