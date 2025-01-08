#include "screenMaterial.h"

ScreenMaterial::ScreenMaterial()
	: mScreenTexture(Texture::createTexture("assets/textures/zero_specular_mask.png", 1))
{
	mType = MaterialType::ScreenMaterial;
}

ScreenMaterial::ScreenMaterial(Texture* screenTexture)
	: mScreenTexture(screenTexture)
{
	mType = MaterialType::ScreenMaterial;

	if (!mScreenTexture)
	{
		mScreenTexture =
			Texture::createTexture("assets/textures/zero_specular_mask.png", 1);
	}
}

ScreenMaterial::~ScreenMaterial()
{
	printf("---- ~ScreenMaterial ----\n");
}

ScreenMaterial* ScreenMaterial::createMaterial()
{
	ScreenMaterial* mat = new ScreenMaterial;
	bookmark.push_back(mat);
	return mat;
}

ScreenMaterial* ScreenMaterial::createMaterial(Texture* screenTexture)
{
	ScreenMaterial* mat = new ScreenMaterial(screenTexture);
	bookmark.push_back(mat);
	return mat;
}