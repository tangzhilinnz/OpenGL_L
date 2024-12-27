#pragma once
#include "light.h"

class DirectionalLight :public Light
{
public:
	DirectionalLight() = default;
	DirectionalLight(const DirectionalLight &) = default;
	DirectionalLight& operator=(const DirectionalLight&) = default;
	~DirectionalLight() = default;

	DirectionalLight(glm::vec3 direction);

	inline void setDirection(glm::vec3 direction)
	{
		mDirection = direction;
	}

	inline glm::vec3& getDirection()
	{ 
		return mDirection;
	}

public:
	glm::vec3 mDirection{ -1.0 };
};