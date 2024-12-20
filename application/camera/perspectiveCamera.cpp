#include "perspectiveCamera.h"


PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
{
	assert(fovy > 0.0f && fovy < 180.0f);
	assert(aspect > 0.0f);
	// The ratio of far/near should ideally be as small as possible to
	// avoid precision
	assert(near > 0.0f && far > near);

	mFovy = fovy; 
	mAspect = aspect;
	mNear = near;
	mFar = far;
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
	//注意：传入的是fovy的角度，需要转化为弧度
	return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
}