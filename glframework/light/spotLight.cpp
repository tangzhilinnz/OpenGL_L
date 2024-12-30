#include "spotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 targetDirection, float innerAngle,
	float outerAngle, float k2, float k1, float kc)
{
	mPosition = position;

	mTargetDirection = targetDirection;
	mInnerAngle = innerAngle;
	mOuterAngle = outerAngle;

	mK2 = k2;
	mK1 = k1;
	mKc = kc;
}