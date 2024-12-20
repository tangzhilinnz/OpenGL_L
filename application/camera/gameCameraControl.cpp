#include "gameCameraControl.h"

void GameCameraControl::OnCursor(double xpos, double ypos)
{
	float deltaX = (float)(xpos - mCurrentX) * mSensitivity;
	float deltaY = (float)(ypos - mCurrentY) * mSensitivity;

	if (mRightMouseDown)
	{
		Pitch(deltaX);
		Yaw(deltaY);
	}

	mCurrentX = (float)xpos;
	mCurrentY = (float)ypos;
}

void GameCameraControl::Update()
{
	//最终移动方向
	glm::vec3 direction(0.0f);

	glm::vec3 front = glm::cross(mCamera->mUp, mCamera->mRight);
	glm::vec3 right = mCamera->mRight;

	if (mKeyMap[GLFW_KEY_W])
	{
		direction += front;
	}
	if (mKeyMap[GLFW_KEY_S])
	{
		direction -= front;
	}
	if (mKeyMap[GLFW_KEY_A])
	{
		direction -= right;
	}
	if (mKeyMap[GLFW_KEY_D])
	{
		direction += right;
	}

	if (direction != glm::vec3(0.0f))
	{
		direction = glm::normalize(direction);
		mCamera->mPosition += direction * mSpeed;
	}
}

void GameCameraControl::Pitch(float angle)
{
	mPitch += angle;
	if (mPitch > 89.0f || mPitch < -89.0f)
	{
		mPitch -= angle;
		return;
	}

	//在gameCameraControl的情况下，pitch不会影响mPosition
	glm::mat4 mat = 
		glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
}

void GameCameraControl::Yaw(float angle)
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
}