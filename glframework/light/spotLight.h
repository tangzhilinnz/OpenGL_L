#pragma once
#include "light.h"
#include "../object.h"

class SpotLight :public Light, public Object
{
public:
	SpotLight() = default;
	SpotLight(const SpotLight&) = default;
	SpotLight& operator=(const SpotLight&) = default;
	~SpotLight() = default;

	SpotLight(glm::vec3 position, glm::vec3 targetDirection, float innerAngle,
		float outerAngle, float k2 = 1.8f, float k1 = 0.7f, float kc = 1.0f);

public:
	glm::vec3	mTargetDirection{ -1.0f };
	float		mInnerAngle{ 0.0f };
	float		mOuterAngle{ 0.0f };

	float mK2{ 1.0f };
	float mK1{ 1.0f };
	float mKc{ 1.0f };
};