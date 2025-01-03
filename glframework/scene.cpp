#include "scene.h"

Scene::Scene()
{
	mType = ObjectType::Scene;
}

Scene::~Scene()
{
	printf("---- ~Scene ----\n");
}