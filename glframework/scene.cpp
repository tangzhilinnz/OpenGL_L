#include "scene.h"

Scene* Scene::createObj()
{
	Scene* scene = new Scene;
	bookmark.push_back(scene);
	return scene;
}

Scene::Scene()
{
	mType = ObjectType::Scene;
}

Scene::~Scene()
{
	printf("---- ~Scene ----\n");
}