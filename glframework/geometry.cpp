#include "geometry.h"
#include <vector>
#include "../wrapper/checkError.h"

#include "../application/stb_image.h"

#include <iostream>

std::vector<Geometry*> Geometry::bookmark;

Geometry::Geometry()
	: mVao(0),
	mPosVbo(0),
	mUVVbo(0),
	mNormalVbo(0),
	mEbo(0),
	mIndicesCount(0)
{}

Geometry::Geometry(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& uvs,
	const std::vector<unsigned int>& indices
)
	: mVao(0),
	mPosVbo(0),
	mUVVbo(0),
	mNormalVbo(0),
	mEbo(0),
	mIndicesCount(0)
{
	mIndicesCount = indices.size();

	//VBO创建
	GL_CALL(glGenBuffers(1, &mPosVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mPosVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float),
		                 positions.data(), GL_STATIC_DRAW));

    GL_CALL(glGenBuffers(1, &mUVVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mUVVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float),
		                 uvs.data(), GL_STATIC_DRAW));

    GL_CALL(glGenBuffers(1, &mNormalVbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float),
		                 normals.data(), GL_STATIC_DRAW));

	//EBO创建
    GL_CALL(glGenBuffers(1, &mEbo));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		                 indices.data(), GL_STATIC_DRAW));

	//VAO创建
    GL_CALL(glGenVertexArrays(1, &mVao));
    GL_CALL(glBindVertexArray(mVao));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mPosVbo));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mUVVbo));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo));
    GL_CALL(glEnableVertexAttribArray(2));
    GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	//加入ebo到当前的vao
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo));

	GL_CALL(glBindVertexArray(0));
}

Geometry::~Geometry()
{
	printf("---- ~Geometry ----\n");

	if (mVao)
	{
		GL_CALL(glDeleteVertexArrays(1, &mVao));
	}
	if (mPosVbo)
	{
		GL_CALL(glDeleteBuffers(1, &mPosVbo));
	}
	if (mUVVbo)
	{
		GL_CALL(glDeleteBuffers(1, &mUVVbo));
	}
	if (mNormalVbo)
	{
		GL_CALL(glDeleteBuffers(1, &mNormalVbo));
	}
	if (mEbo)
	{
		GL_CALL(glDeleteBuffers(1, &mEbo));
	}

	// Set this instance in the static bookmark container to nullptr
	for (auto& geo : bookmark)
	{
		if (geo == this)
		{
			geo = nullptr;
			break;
		}
	}
}

Geometry* Geometry::createGeometry(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& uvs,
	const std::vector<unsigned int>& indices)
{
	Geometry* geometry = new Geometry(positions, normals, uvs, indices);
	bookmark.push_back(geometry);
	return geometry;
}

Geometry* Geometry::createScreenPlane()
{
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;

	// In the order of vertices: Top Left, Bottom Left, Bottom Right,
	// and Top Right
	float positions[] = {
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
	};

	float uvs[] = {
        0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	//创建vao vbo等
	GLuint& posVbo = geometry->mPosVbo;
	GLuint& uvVbo = geometry->mUVVbo;

	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &uvVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &geometry->mEbo));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	GL_CALL(glGenVertexArrays(1, &geometry->mVao));
	GL_CALL(glBindVertexArray(geometry->mVao));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));

	GL_CALL(glBindVertexArray(0));

	bookmark.push_back(geometry);
	return geometry;
}

Geometry* Geometry::createBox(float size, bool isLeftHandedCrossUV)
{
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 36;

	float halfSize = size / 2.0f;

	float positions[] = {
		// Front face
		-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize,
		halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize,
		// Back face
		-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		// Top face
		-halfSize, halfSize, halfSize, halfSize, halfSize, halfSize,
		halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		// Bottom face
		-halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
		// Right face
		halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize,
		halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
		// Left face
		-halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize,
		-halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};

	float uvs0[] = {
		0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f,
		0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f,
		0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f,
		0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f,
		0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f,
		0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f,
	};

	// stbi_set_flip_vertically_on_load(false);
	//float uvs1[] = {
	//	0.9999f, 0.6665f,  0.7501f, 0.6665f,  0.7501f, 0.3334f,  0.9999f, 0.3334f,
	//	0.2501f, 0.6665f,  0.2501f, 0.3334f,  0.4999f, 0.3334f,  0.4999f, 0.6665f,
	//	0.2501f, 0.6667f,  0.4999f, 0.6667f,  0.4999f, 0.9999f,  0.2501f, 0.9999f,
	//	0.2501f, 0.0001f,  0.4999f, 0.0001f,  0.4999f, 0.3332f,  0.2501f, 0.3332f,
	//	0.7499f, 0.6665f,  0.5001f, 0.6665f,  0.5001f, 0.3334f,  0.7499f, 0.3334f,
	//	0.2499f, 0.6665f,  0.0001f, 0.6665f,  0.0001f, 0.3334f,  0.2499f, 0.3334f,
	//};

	// stbi_set_flip_vertically_on_load(true);
	float uvs1[] = {
		0.9999f, 0.3334f,  0.7501f, 0.3334f,  0.7501f, 0.6665f,  0.9999f, 0.6665f,
		0.2501f, 0.3334f,  0.2501f, 0.6665f,  0.4999f, 0.6665f,  0.4999f, 0.3334f,
		0.2501f, 0.9999f,  0.4999f, 0.9999f,  0.4999f, 0.6667f,  0.2501f, 0.6667f,
		0.2501f, 0.3332f,  0.4999f, 0.3332f,  0.4999f, 0.0001f,  0.2501f, 0.0001f,
		0.7499f, 0.3334f,  0.5001f, 0.3334f,  0.5001f, 0.6665f,  0.7499f, 0.6665f,
		0.2499f, 0.3334f,  0.0001f, 0.3334f,  0.0001f, 0.6665f,  0.2499f, 0.6665f,
	};

	float normals[] = {
		//前面
		0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f,
		//后面
		0.0f, 0.0f, -1.0f,     0.0f, 0.0f, -1.0f,     0.0f, 0.0f, -1.0f,     0.0f, 0.0f, -1.0f,
		//上面
		0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,
		//下面
		0.0f, -1.0f, 0.0f,     0.0f, -1.0f, 0.0f,     0.0f, -1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		//右面
		1.0f, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f,
		//左面
		-1.0f, 0.0f, 0.0f,     -1.0f, 0.0f, 0.0f,     -1.0f, 0.0f, 0.0f,     -1.0f, 0.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,         2, 3, 0,        // Front face
		4, 5, 6,         6, 7, 4,        // Back face
		8, 9, 10,        10, 11, 8,      // Top face
		12, 13, 14,      14, 15, 12,     // Bottom face
		16, 17, 18,      18, 19, 16,     // Right face
		20, 21, 22,      22, 23, 20      // Left face
	};

	//2 VBO创建
	GLuint& posVbo = geometry->mPosVbo;
	GLuint& uvVbo = geometry->mUVVbo;
	GLuint& normalVbo = geometry->mNormalVbo;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	if (isLeftHandedCrossUV)
	{
		GL_CALL(glGenBuffers(1, &uvVbo));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs1), uvs1, GL_STATIC_DRAW));
	}
	else
	{
		GL_CALL(glGenBuffers(1, &uvVbo));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs0), uvs0, GL_STATIC_DRAW));
	}

	GL_CALL(glGenBuffers(1, &normalVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW));

	//3 EBO创建
	GL_CALL(glGenBuffers(1, &geometry->mEbo));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	//4 VAO创建
	GL_CALL(glGenVertexArrays(1, &geometry->mVao));
	GL_CALL(glBindVertexArray(geometry->mVao));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	//5.4 加入ebo到当前的vao
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));

	GL_CALL(glBindVertexArray(0));

	bookmark.push_back(geometry);
	return geometry;
}

Geometry* Geometry::createSphere(float radius, int numLatBelts, int numLongZones)
{
	Geometry* geometry = new Geometry();

	//目标：1 位置 2 uv 3 索引
	//1 主要变量声明
	std::vector<GLfloat> positions{};
	std::vector<GLfloat> uvs{};
	std::vector<GLfloat> normals{};
	std::vector<GLuint> indices{};

	//2 通过两层循环（纬线在外，经线在内）->位置、uv
	for (int i = 0; i <= numLatBelts; i++)
	{
		for (int j = 0; j <= numLongZones; j++)
		{
			float phi = i * glm::pi<float>() / numLatBelts;
			float theta = j * 2 * glm::pi<float>() / numLongZones;

			float y = radius * cosf(phi);
			float x = radius * sinf(phi) * cosf(theta);
			float z = radius * sinf(phi) * sinf(theta);

			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);

			float u = 1.0f - (float)j / (float)numLongZones; // lantitude
			float v = 1.0f - (float)i / (float)numLatBelts;  // longitude

			uvs.push_back(u);
			uvs.push_back(v);

			//注意：法线方向没有问题，法线的长度不为1
			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}
	}

	//3 通过两层循环（这里没有=号）->顶点索引
	for (int i = 0; i < numLatBelts; i++)
	{
		for (int j = 0; j < numLongZones; j++)
		{
			// The total number of intersection points between all longitudes and a single
			// arbitrary latitude is numLongZones + 1
			int p1 = i * (numLongZones + 1) + j;
			int p2 = p1 + numLongZones + 1;
			int p3 = p1 + 1;
			int p4 = p2 + 1;

			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);

			indices.push_back(p3);
			indices.push_back(p2);
			indices.push_back(p4);
		}
	}

	//4 生成vbo与vao
	GLuint& posVbo = geometry->mPosVbo;
	GLuint& uvVbo = geometry->mUVVbo;
	GLuint& normalVbo = geometry->mNormalVbo;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &uvVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &normalVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &geometry->mEbo));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW));

	GL_CALL(glGenVertexArrays(1, &geometry->mVao));
	GL_CALL(glBindVertexArray(geometry->mVao));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));

	GL_CALL(glBindVertexArray(0));

	geometry->mIndicesCount = indices.size();

	bookmark.push_back(geometry);
	return geometry;
}

Geometry* Geometry::createPlane(float width, float height, float urep, float vrep)
{
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;

	float halfW = width / 2.0f;
	float halfH = height / 2.0f;

	float positions[] = {
		-halfW,   -halfH,   0.0f,
		halfW,    -halfH,   0.0f,
		halfW,    halfH,    0.0f,
		-halfW,   halfH,    0.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		urep, 0.0f,
		urep, vrep,
		0.0f, vrep
	};

	float normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//2 VBO创建
	GLuint& posVbo = geometry->mPosVbo;
	GLuint& uvVbo = geometry->mUVVbo;
	GLuint& normalVbo = geometry->mNormalVbo;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &uvVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &normalVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW));

	//3 EBO创建
	GL_CALL(glGenBuffers(1, &geometry->mEbo));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	//4 VAO创建
	GL_CALL(glGenVertexArrays(1, &geometry->mVao));
	GL_CALL(glBindVertexArray(geometry->mVao));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	//5.4 加入ebo到当前的vao
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));

	GL_CALL(glBindVertexArray(0));

	bookmark.push_back(geometry);
	return geometry;
}

Geometry* Geometry::createSimpleTerrain(const char* heightmapFile, float len, float maxHeight,
	float yTrans, float urep, float vrep) 
{
	Geometry* geometry = new Geometry();

	int width, height, channels;
	// Load as grayscale
	stbi_set_flip_vertically_on_load(true);

	unsigned char* hgtmap = stbi_load(heightmapFile, &width, &height, &channels, 1);
	assert(hgtmap && width == height); // Ensure valid heightmap

	size_t side = width;
	size_t nVerts = side * side;
	size_t nTri = 2 * (side - 1) * (side - 1);

	std::vector<GLfloat> positions{};
	std::vector<GLfloat> uvs{};
	std::vector<GLfloat> normals{};
	std::vector<GLuint>  indices{};

	float triangleSide = len / (side - 1);

	// Fill vertex data
	for (size_t z = 0; z < side; z++)
	{
		for (size_t x = 0; x < side; x++)
		{
			// Extract height from heightmap from the grayscale image
			// When using STB Image (stb_image.h), the image is loaded with the
			// top-left corner as the origin, but OpenGL expects the texture's
			// origin to be in the bottom-left corner.
			// To fix this, stbi_set_flip_vertically_on_load(true); flips the
			// image vertically before loading it into OpenGL.However, if you're
			// manually computing texture coordinates or accessing heightmap data,
			// you need to adjust the indexing accordingly.
			// (float)z / (side - 1) * vrep --> vrep - (float)z / (side - 1) * vrep
			// z * side + x --> (side - 1 - z) * side + x
			size_t pixelIndex = /*channels * */((side - 1 - z) * side + x);
			uint8_t h_val = hgtmap[pixelIndex];                    // Extract the R channel
			float height = (h_val / 255.0f) * maxHeight + yTrans;  // Normalize to [0, maxheight]
			positions.push_back(-0.5f * len + x * triangleSide);   // X Position
			positions.push_back(height);                           // Y Position (height)
			positions.push_back(-0.5f * len + z * triangleSide);   // Z Position
			uvs.push_back((float)x / (side - 1) * urep);           // U Texture Coordinate
			uvs.push_back(vrep - (float)z / (side - 1) * vrep);    // V Texture Coordinate
		}
	}

	// Fill index data
	for (size_t z = 0; z < side - 1; z++)
	{
		for (size_t x = 0; x < side - 1; x++)
		{
			// Define indices for two triangles in a grid cell
			size_t topLeft = z * side + x;
			size_t topRight = topLeft + 1;
			size_t bottomLeft = topLeft + side;
			size_t bottomRight = bottomLeft + 1;

			indices.push_back(topRight);
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	// Fill normal data
	std::vector<glm::vec3> normalsTmp(nVerts, glm::vec3(0.0f, 0.0f, 0.0f));
	for (size_t i = 0; i < nTri; i++)
	{
		// Get indices
		size_t i0 = indices[i * 3 + 0];
		size_t i1 = indices[i * 3 + 1];
		size_t i2 = indices[i * 3 + 2];

		// Get vertex positions
		glm::vec3 v0(positions[i0 * 3], positions[i0 * 3 + 1], positions[i0 * 3 + 2]);
		glm::vec3 v1(positions[i1 * 3], positions[i1 * 3 + 1], positions[i1 * 3 + 2]);
		glm::vec3 v2(positions[i2 * 3], positions[i2 * 3 + 1], positions[i2 * 3 + 2]);

		// Compute face normal
		glm::vec3 e0 = v1 - v0;
		glm::vec3 e1 = v2 - v0;

		glm::vec3 faceNormal = glm::normalize(glm::cross(e0, e1));

		// Compute area-based weight (optional)
		float areaWeight = glm::length(glm::cross(e0, e1)) * 0.5f;

		// Weighted normal contribution
		normalsTmp[i0] += faceNormal * areaWeight;
		normalsTmp[i1] += faceNormal * areaWeight;
		normalsTmp[i2] += faceNormal * areaWeight;
	}

	// Normalize all vertex normals
	for (size_t i = 0; i < nVerts; i++)
	{
		assert(glm::length(normalsTmp[i]) > 0.0f); // Avoid division by zero
		normalsTmp[i] = glm::normalize(normalsTmp[i]);

		normals.push_back(normalsTmp[i].x);
		normals.push_back(normalsTmp[i].y);
		normals.push_back(normalsTmp[i].z);
	}

	stbi_image_free(hgtmap); // Free image memory

	//4 生成vbo与vao
	GLuint& posVbo = geometry->mPosVbo;
	GLuint& uvVbo = geometry->mUVVbo;
	GLuint& normalVbo = geometry->mNormalVbo;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &uvVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &normalVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &geometry->mEbo));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW));

	GL_CALL(glGenVertexArrays(1, &geometry->mVao));
	GL_CALL(glBindVertexArray(geometry->mVao));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));

	GL_CALL(glBindVertexArray(0));

	geometry->mIndicesCount = indices.size();

	bookmark.push_back(geometry);
	return geometry;
}

void Geometry::destroyAllInstances()
{
	for (Geometry* instance : bookmark)
	{
		if (instance) delete instance;
	}
	bookmark.clear();
}