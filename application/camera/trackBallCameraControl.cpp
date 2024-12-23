#include "trackBallCameraControl.h"
#include <iostream>

void TrackBallCameraControl::OnCursor(double xpos, double ypos)
{
	if (mLeftMouseDown)
	{
		//��������ĸ������
		//1 ���㾭�߸�γ����ת�������Ƕ�(�������п���)
		float deltaX = (float)(xpos - mCurrentX) * mSensitivity;
		float deltaY = (float)(ypos - mCurrentY) * mSensitivity;

		//2 �ֿ�pitch��yaw���Լ���

		Pitch(-deltaY);
		Yaw(-deltaX);
	}
	else if (mMiddleMouseDown)
	{
		float deltaX = (float)(xpos - mCurrentX) * mMoveSpeed;
		float deltaY = (float)(ypos - mCurrentY) * mMoveSpeed;

		mCamera->mPosition += mCamera->mUp * deltaY;
		mCamera->mPosition -= mCamera->mRight * deltaX;
	}

	mCurrentX = (float)xpos;
	mCurrentY = (float)ypos;
}

void TrackBallCameraControl::OnScroll(double offset)
{
	mCamera->Scale(mScaleSpeed * (float)offset);
}

void TrackBallCameraControl::Pitch(float angle)
{

	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);

	//Ӱ�쵱ǰ�����up������λ��
	mCamera->mUp = glm::vec3(mat * glm::vec4(mCamera->mUp, 0.0f));//vec4����vec3������xyz
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::Yaw(float angle)
{
	//������������ϵ��y����ת
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}