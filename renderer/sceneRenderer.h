#pragma once

#include "openGLRenderer.h"
#include "../glframework/core.h"
#include "../glframework/object.h"
#include "../glframework/scene.h"
#include "../application/assimpLoader.h"

#include <assert.h>
#include <stack>

// Abstract scene Class
class SceneRenderer : public OpenGLRenderer
{
public:
	// Need to rewrite in a implementation class
	virtual void meshRender(Object* object)
	{
		assert(false && "Function not implemented yet.");
	}

	// Perform DFS on a scene tree using iterative version
	void objectRender(Object* root)
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
				this->meshRender(object);
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

	void render() override
	{
		this->doTransform();

		//清理画布 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//将scene当作根节点开iteratie渲染
		this->objectRender(scene);
	}

	~SceneRenderer()
	{
		printf("---- ~SceneRenderer ----\n");

		if (scene)
		{
			AssimpLoader::destroy(scene);
		}
	}

protected:
	virtual void doTransform() {};

protected:
	Scene* scene{nullptr};
};