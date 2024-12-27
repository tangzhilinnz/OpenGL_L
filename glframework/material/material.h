#pragma once
#include "../core.h"

//使用C++的枚举类型
enum class MaterialType
{
	PhongMaterial
};

class Material
{
public:
	Material() = default;
	Material(const Material&) = default;
	Material& operator=(const Material&) = default;
	~Material() = default;

public:
	MaterialType mType;
};