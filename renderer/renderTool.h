#pragma once

#include "openGLRenderer.h"
#include "../glframework/core.h"
#include "../glframework/object.h"
#include "../glframework/scene.h"
#include "../glframework/geometry.h"
#include "../glframework/texture.h"
#include "../glframework/state.h"
#include "../glframework/material/material.h"

#include <assert.h>
#include <stack>

class RenderTool
{
public:
	// Perform DFS on a scene tree using iterative version
	static void objectRender(Object* root, OpenGLRenderer* rdr)
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
				rdr ->meshRendering(object);
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

	static void sceneClear()
	{
		State::destroyAllInstances();
		Object::destroyAllInstances();
		Material::destroyAllInstances();
		Geometry::destroyAllInstances();
		Texture::clearCache();
	}
};