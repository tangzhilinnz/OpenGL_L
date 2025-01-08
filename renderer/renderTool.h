#pragma once

#include "openGLRenderer.h"
#include "../glframework/core.h"
#include "../glframework/object.h"
#include "../glframework/scene.h"
#include "../glframework/geometry.h"
#include "../glframework/texture.h"
#include "../glframework/mesh.h"
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
	static void objectRender(Object* root, OpenGLRenderer* rdr);
	static void objectSortedRender(Object* root, OpenGLRenderer* rdr);
	static void sceneClear();
	static void enableModelBlend(Object* root);
	static void setModelOpcity(Object* root, float opacity = 1.0f);
	static void disableModelBlend(Object* root);

	static void setModelUniformMaterial(Object* root, Material* mat);
private:
	static std::vector<Mesh*>	mOpacityObjects;
	static std::vector<Mesh*>	mTransparentObjects;

	static void separateMesh(Object* root);
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