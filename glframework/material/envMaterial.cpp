#include "envMaterial.h"

EnvMaterial::EnvMaterial()
	: mEnvTex(nullptr)
{
	mType = MaterialType::EnvMaterial;
}

EnvMaterial::EnvMaterial(Texture* envTex)
	: mEnvTex(envTex)
{
	mType = MaterialType::PhongMaterial;
}

EnvMaterial::~EnvMaterial()
{
	printf("---- ~EnvMaterial ----\n");
}

EnvMaterial* EnvMaterial::createMaterial()
{
	EnvMaterial* mat = new EnvMaterial;
	bookmark.push_back(mat);
	return mat;
}

EnvMaterial* EnvMaterial::createMaterial(Texture* envTex)
{
	EnvMaterial* mat = new EnvMaterial(envTex);
	bookmark.push_back(mat);
	return mat;
}
