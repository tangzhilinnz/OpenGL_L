#pragma once

#include "../../glframework/core.h"

class Camera
{
public:
	Camera() = default;
	Camera(const Camera&) = default;
	Camera& operator=(const Camera&) = default;
	virtual ~Camera() = default;

	glm::mat4 GetViewMatrix() const;
	
	virtual glm::mat4 GetProjectionMatrix() const;

public:
	glm::vec3 mPosition{ 0.0f, 0.0f, 1.0f };
	glm::vec3 mUp{ 0.0f, 1.0f, 0.0f };
	glm::vec3 mRight{ 1.0f, 0.0f, 0.0f };
};