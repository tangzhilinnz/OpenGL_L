#pragma once
#include "../core.h"
#include <vector>

//使用C++的枚举类型
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

public:
	void setDepthTest(bool	depthTest, GLenum depthFunc = GL_LESS)
	{
		mDepthTest = depthTest;
		mDepthFunc = depthFunc;
	}

	void setDepthWrite(bool depthWrite)
	{
		mDepthWrite = depthWrite;
	}

	void setPolygonOffset(bool polygonOffset, unsigned int polygonOffsetType,
		float factor, float unit)
	{
		mPolygonOffset = polygonOffset;
		mPolygonOffset = polygonOffsetType;
		mFactor = factor;
		mUnit = unit;
	}

	//深度检测相关
	bool	mDepthTest{ true };
	bool	mDepthWrite{ true };
	GLenum	mDepthFunc{ GL_LESS };

	//polygonOffset相关
	bool			mPolygonOffset{ false };
	unsigned int	mPolygonOffsetType{ GL_POLYGON_OFFSET_FILL };
	float			mFactor{ 0.0f };
	float			mUnit{ 0.0f };

protected:
	// Static bookmark to store instances
	static std::vector<Material*> bookmark;
};