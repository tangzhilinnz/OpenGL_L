#pragma once

#include "core.h"
#include "object.h"

class Scene :public Object
{
public:
	static Scene* createObj();
	
	Scene();
	Scene(const Scene&) = default;
	Scene& operator=(const Scene&) = default;
	~Scene();
};