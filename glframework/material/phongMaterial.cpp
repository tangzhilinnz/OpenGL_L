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

PhongMaterial::~PhongMaterial()
{
	printf("---- ~PhongMaterial ----\n");
}

PhongMaterial* PhongMaterial::createMaterial()
{
	PhongMaterial* mat = new PhongMaterial;
	bookmark.push_back(mat);
	return mat;
}

PhongMaterial* PhongMaterial::createMaterial(
	Texture* diffuse, Texture* specularMask, float shiness)
{
	PhongMaterial* mat = new PhongMaterial(diffuse, specularMask, shiness);
	bookmark.push_back(mat);
	return mat;
}

