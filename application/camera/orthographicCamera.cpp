#include "orthographicCamera.h"

OrthographicCamera::OrthographicCamera(float l, float r, float t, float b, float n, float f)
{
	assert(l < r && t > b && n != f);

	mLeft = l;
	mRight = r;
	mTop = t;
	mBottom = b;
	mNear = n;
	mFar = f;
}

glm::mat4 OrthographicCamera::GetProjectionMatrix() const
{
	float scale = std::pow(2.0f, mScale);
	return glm::ortho(scale * mLeft, scale * mRight, scale * mBottom, scale * mTop, mNear, mFar);
}

void OrthographicCamera::Scale(float deltaScale)
{
	mScale += deltaScale;
}