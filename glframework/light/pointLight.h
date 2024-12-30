#pragma once
#include "light.h"
#include "../object.h"

class PointLight :public Light, public Object
{
public:
	PointLight() = default;
	PointLight(const PointLight&) = default;
	PointLight& operator=(const PointLight&) = default;
	~PointLight() = default;

	PointLight(glm::vec3 position, float k2 = 1.8f, float k1 = 0.7f, float kc = 1.0f);

public:
	float mK2 { 1.0f };
	float mK1 { 1.0f };
	float mKc { 1.0f };
};