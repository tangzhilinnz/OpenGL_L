#pragma once

#include "core.h"

class Geometry
{
public:
	Geometry() = default;
	Geometry(const Geometry&) = default;
	Geometry& operator=(const Geometry&) = default;
	~Geometry();

	static Geometry* createBox(float size);
	static Geometry* createSphere(float radius, int numLatBelts=60, int numLongZones=60);

	GLuint getVao() const { return mVao; }
	uint32_t getIndicesCount() const { return mIndicesCount; }

private:
	GLuint mVao{0};
	GLuint mPosVbo{ 0 };
	GLuint mUVVbo{ 0 };
	GLuint mEbo{ 0 };

	uint32_t mIndicesCount{ 0 };
};