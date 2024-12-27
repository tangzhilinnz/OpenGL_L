#include "phongMaterial.h"

PhongMaterial::PhongMaterial()
{
	mType = MaterialType::PhongMaterial;
}


PhongMaterial::PhongMaterial(Texture* diffuse, float shiness)
    : mDiffuse(diffuse),
	mShiness(shiness)
{
	mType = MaterialType::PhongMaterial;
}

