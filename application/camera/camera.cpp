#include "camera.h"

glm::mat4 Camera::GetViewMatrix() const
{
	// lookat
	//-eye:相机位置（使用mPosition）
	//-center：看向世界坐标的哪个点
	//-top：穹顶（使用mUp替代）
	glm::vec3 front = glm::cross(mUp, mRight);
	glm::vec3 center = mPosition + front;

	// The matrix generated by glm::lookAt is essentially the inverse of
	// the camera's translation and rotation matrix
	return glm::lookAt(mPosition, center, mUp);
}

virtual glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::identity<glm::mat4>();
}