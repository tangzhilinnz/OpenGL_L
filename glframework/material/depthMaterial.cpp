#include "depthMaterial.h"

DepthMaterial::DepthMaterial()
{
	mType = MaterialType::DepthMaterial;
}

DepthMaterial::~DepthMaterial()
{
	printf("---- ~DepthMaterial ----\n");
}

DepthMaterial* DepthMaterial::createMaterial()
{
	DepthMaterial* mat = new DepthMaterial;
	bookmark.push_back(mat);
	return mat;
}