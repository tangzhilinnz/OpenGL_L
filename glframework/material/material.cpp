#include "material.h"


std::vector<Material*> Material::bookmark;

Material::Material()
	: mType(MaterialType::DefaultMaterial),
	mOpacity(1.0f)
{}

Material:: ~Material()
{
	printf("---- ~Material ----\n");

	for (auto& geo : bookmark)
	{
		if (geo == this)
		{
			geo = nullptr;
			break;
		}
	}
}

void Material::destroyAllInstances()
{
	for (Material* instance : bookmark)
	{
		if (instance) delete instance;
	}
	bookmark.clear();
}