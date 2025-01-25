#pragma once
#include "../core.h"
#include <vector>

//使用C++的枚举类型
enum class MaterialType
{
	PhongMaterial,
	WhiteMaterial,
	DepthMaterial,
	EnvMaterial,
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

	void setOpacity(float opacity) { mOpacity = opacity; }
	float getOpacity() const { return mOpacity; }

public:
	MaterialType mType;

protected:
	float mOpacity{ 1.0f };

protected:
	// Static bookmark to store instances
	static std::vector<Material*> bookmark;
};