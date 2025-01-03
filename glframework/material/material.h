#pragma once
#include "../core.h"

//ʹ��C++��ö������
enum class MaterialType
{
	PhongMaterial,
	WhiteMaterial
};

class Material
{
public:
	Material() = default;
	Material(const Material&) = default;
	Material& operator=(const Material&) = default;
	virtual ~Material();

public:
	MaterialType mType;
};