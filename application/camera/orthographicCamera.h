#pragma once

#include "camera.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera() = default;
	OrthographicCamera(float l, float r, float t, float b, float n, float f);
	OrthographicCamera(const OrthographicCamera&) = default;
	OrthographicCamera& operator=(const OrthographicCamera&) = default;
	~OrthographicCamera() = default;

	glm::mat4 GetProjectionMatrix() const override;

private:
	float mLeft{ 0.0f };
	float mRight{ 0.0f };
	float mTop{ 0.0f };
	float mBottom{ 0.0f };
	float mNear{ 0.0f };
	float mFar{ 0.0f };
};