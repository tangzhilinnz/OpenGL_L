#pragma once
#include "../core.h"
#include <vector>

//ʹ��C++��ö������
enum class MaterialType
{
	PhongMaterial,
	WhiteMaterial,
	DepthMaterial,
	DefaultMaterial
};

class Material
{
public:
	static Material* createMaterial() {}

	Material();
	Material(const Material&) = default;
	Material& operator=(const Material&) = default;
	virtual ~Material();

	// Uniform method to destroy all instances
	static void destroyAllInstances();
	const std::vector<Material*>& getInstances() { return bookmark; }

public:
	MaterialType mType;

protected:
	// Static bookmark to store instances
	static std::vector<Material*> bookmark;
};