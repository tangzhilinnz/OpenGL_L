#pragma once
#include "../core.h"

//ʹ��C++��ö������
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