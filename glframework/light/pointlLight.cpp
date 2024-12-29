#include "pointLight.h"

PointLight::PointLight(glm::vec3 position, float k2, float k1, float kc)
{
	mPosition = position;

	mK2 = k2;
	mK1 = k1;
	mKc = kc;
}