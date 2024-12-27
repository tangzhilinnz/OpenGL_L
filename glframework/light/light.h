#pragma once
#include "../core.h"

class Light
{
public:
	Light() = default;
	Light(const Light&) = default;
	Light& operator=(const Light&) = default;
	~Light() = default;

	inline void setColor(glm::vec3 color)
	{
		mColor = color;
	}

	inline void setSpecularIntensity(float intensity)
	{
		mSpecularIntensity = intensity;
	}

	inline glm::vec3& getColor()
	{
		return mColor;
	}

	inline float getSpecularIntensity()
	{
		return mSpecularIntensity;
	}

protected:
	glm::vec3	mColor{ 1.0f };
	float		mSpecularIntensity{ 1.0f };
};