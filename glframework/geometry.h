#pragma once

#include "core.h"

#include <vector>

class Geometry
{
public:
	Geometry(const Geometry&) = default;
	Geometry& operator=(const Geometry&) = default;
	virtual ~Geometry();

	static Geometry* createBox(float size);
	static Geometry* createSphere(float radius, int numLatBelts=60, int numLongZones=60);
	static Geometry* createPlane(float width, float height);
	static Geometry* createGeometry(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& uvs,
		const std::vector<unsigned int>& indices);

	// Uniform method to destroy all instances
	static void destroyAllInstances();
	const std::vector<Geometry*>& getInstances() { return bookmark; }

	GLuint getVao() const { return mVao; }
	uint32_t getIndicesCount() const { return mIndicesCount; }

private:
	GLuint mVao{0};
	GLuint mPosVbo{ 0 };
	GLuint mUVVbo{ 0 };
	GLuint mNormalVbo{ 0 };
	GLuint mEbo{ 0 };

	uint32_t mIndicesCount{ 0 };

	// Static bookmark to store instances
	static std::vector<Geometry*> bookmark;

public:
	Geometry();

	Geometry(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& uvs,
		const std::vector<unsigned int>& indices
	);
};