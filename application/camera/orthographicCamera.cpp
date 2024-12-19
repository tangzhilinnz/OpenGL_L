#include "orthographicCamera.h"

OrthographicCamera::OrthographicCamera(float l, float r, float t, float b, float n, float f)
{
	assert(l < r && t > b && n < f);

	mLeft = l;
	mRight = r;
	mTop = t;
	mBottom = b;
	mNear = n;
	mFar = f;
}

glm::mat4 OrthographicCamera::GetProjectionMatrix()
{
	return glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
}