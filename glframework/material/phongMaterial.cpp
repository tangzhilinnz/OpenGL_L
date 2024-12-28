#include "phongMaterial.h"

PhongMaterial::PhongMaterial()
{
	mType = MaterialType::PhongMaterial;
}


PhongMaterial::PhongMaterial(Texture* diffuse, Texture* specularMask, float shiness)
    : mDiffuse(diffuse),
	mSpecularMask(specularMask),
	mShiness(shiness)
{
	mType = MaterialType::PhongMaterial;
}

