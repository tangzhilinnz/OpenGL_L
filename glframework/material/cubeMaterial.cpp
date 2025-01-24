#include "cubeMaterial.h"

CubeMaterial::CubeMaterial()
{
	const char* paths[] = {
		"assets/textures/skybox/right.jpg",
		"assets/textures/skybox/left.jpg",
		"assets/textures/skybox/top.jpg",
		"assets/textures/skybox/bottom.jpg",
		"assets/textures/skybox/back.jpg",
		"assets/textures/skybox/front.jpg"
	};

	mCubeMap = Texture::createCubeMapTexture(paths, 0);
	mType = MaterialType::CubeMaterial;
}

CubeMaterial::CubeMaterial(Texture* cubeMap)
	: mCubeMap(cubeMap)
{
	if (!cubeMap)
	{
		const char* paths[] = {
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/back.jpg",
			"assets/textures/skybox/front.jpg"
		};

		mCubeMap = Texture::createCubeMapTexture(paths, 0);
	}

	mType = MaterialType::PhongMaterial;
}

CubeMaterial::~CubeMaterial()
{
	printf("---- ~CubeMaterial ----\n");
}

CubeMaterial* CubeMaterial::createMaterial()
{
	CubeMaterial* mat = new CubeMaterial;
	bookmark.push_back(mat);
	return mat;
}

CubeMaterial* CubeMaterial::createMaterial(Texture* cubeMap)
{
	CubeMaterial* mat = new CubeMaterial(cubeMap);
	bookmark.push_back(mat);
	return mat;
}
