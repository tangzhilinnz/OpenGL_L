#include "whiteMaterial.h"

WhiteMaterial::WhiteMaterial()
{
	mType = MaterialType::WhiteMaterial;
}

WhiteMaterial::~WhiteMaterial()
{
	printf("---- ~WhiteMaterial ----\n");
}

WhiteMaterial* WhiteMaterial::createMaterial()
{
	WhiteMaterial* mat = new WhiteMaterial;
	bookmark.push_back(mat);
	return mat;
}