#pragma once

#include "core.h"
#include "object.h"

class Scene :public Object
{
public:
	Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	~Scene();
};