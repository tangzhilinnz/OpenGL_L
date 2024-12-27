#pragma once
#include "light.h"

class AmbientLight :public Light
{
public:
	AmbientLight() = default;
	AmbientLight(const AmbientLight&) = default;
	AmbientLight& operator=(const AmbientLight&) = default;
	~AmbientLight() = default;
};