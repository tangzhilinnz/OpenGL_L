#include "phongMaterial.h"

PhongMaterial::PhongMaterial()
    : mDiffuse(Texture::createTexture("assets/textures/defaultTexture.jpg", 0)),
    mSpecularMask(Texture::createTexture("assets/textures/zero_specular_mask.png", 1)),
    mOpcityMask(Texture::createTexture("assets/textures/default_opacity_mask.png", 2)),
	mShiness(1.0f)
{
	mType = MaterialType::PhongMaterial;
}

PhongMaterial::PhongMaterial(Texture* diffuse, Texture* specularMask, Texture* opcityMask, float shiness)
    : mDiffuse(diffuse),
	mSpecularMask(specularMask),
	mOpcityMask(opcityMask),
	mShiness(shiness)
{
	if (!diffuse)
	{
		mDiffuse = Texture::createTexture("assets/textures/defaultTexture.jpg", 0);
	}

	if (!specularMask)
	{
		mDiffuse = Texture::createTexture("assets/textures/zero_specular_mask.png", 1);
	}

	if (!opcityMask)
	{
		mOpcityMask = Texture::createTexture("assets/textures/default_opacity_mask.png", 2);
	}

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
	Texture* diffuse, Texture* specularMask, Texture* opcityMask, float shiness)
{
	PhongMaterial* mat = new PhongMaterial(diffuse, specularMask, opcityMask, shiness);
	bookmark.push_back(mat);
	return mat;
}

