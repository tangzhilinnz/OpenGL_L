#pragma once

#include "camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera() = default;
	PerspectiveCamera(float fovy, float aspect, float near, float far);
	PerspectiveCamera(const PerspectiveCamera&) = default;
	PerspectiveCamera& operator=(const PerspectiveCamera&) = default;
	~PerspectiveCamera() = default;

	glm::mat4 GetProjectionMatrix() const override;

	void Scale(float deltaScale) override;

private:
	float mFovy{ 0.0f };
	float mAspect{ 0.0f };
};