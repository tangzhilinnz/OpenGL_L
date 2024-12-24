#include "geometry.h"
#include <vector>
#include "../wrapper/checkError.h"

#include <iostream>

Geometry::~Geometry()
{
	printf("---- ~Geometry ----\n");

	if (mVao != 0)
	{
		GL_CALL(glDeleteVertexArrays(1, &mVao));
	}
	if (mPosVbo != 0)
	{
		GL_CALL(glDeleteBuffers(1, &mPosVbo));
	}
	if (mUVVbo != 0)
	{
		GL_CALL(glDeleteBuffers(1, &mUVVbo));
	}
	if (mNormalVbo != 0)
	{
		GL_CALL(glDeleteBuffers(1, &mNormalVbo));
	}
	if (mEbo != 0)
	{
		GL_CALL(glDeleteBuffers(1, &mEbo));
	}
}

Geometry* Geometry::createBox(float size)
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

	float uvs[] = {
		0.0, 0.0,     1.0, 0.0,     1.0, 1.0,     0.0, 1.0,
		0.0, 0.0,     1.0, 0.0,     1.0, 1.0,     0.0, 1.0,
		0.0, 0.0,     1.0, 0.0,     1.0, 1.0,     0.0, 1.0,
		0.0, 0.0,     1.0, 0.0,     1.0, 1.0,     0.0, 1.0,
		0.0, 0.0,     1.0, 0.0,     1.0, 1.0,     0.0, 1.0,
		0.0, 0.0,     1.0, 0.0,     1.0, 1.0,     0.0, 1.0,
	};

	float normals[] = {
		//ǰ��
		0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f,
		//����
		0.0f, 0.0f, -1.0f,     0.0f, 0.0f, -1.0f,     0.0f, 0.0f, -1.0f,     0.0f, 0.0f, -1.0f,
		//����
		0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,
		//����
		0.0f, -1.0f, 0.0f,     0.0f, -1.0f, 0.0f,     0.0f, -1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		//����
		1.0f, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f,      1.0f, 0.0f, 0.0f,
		//����
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

	//2 VBO����
	GLuint& posVbo = geometry->mPosVbo;
	GLuint& uvVbo = geometry->mUVVbo;
	GLuint& normalVbo = geometry->mNormalVbo;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	GL_CALL(glGenBuffers(1, &uvVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW));

	glGenBuffers(1, &normalVbo);
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW));

	//3 EBO����
	GL_CALL(glGenBuffers(1, &geometry->mEbo));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	//4 VAO����
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

	//5.4 ����ebo����ǰ��vao
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));

	GL_CALL(glBindVertexArray(0));

	return geometry;
}


Geometry* Geometry::createSphere(float radius, int numLatBelts, int numLongZones)
{
	Geometry* geometry = new Geometry();

	//Ŀ�꣺1 λ�� 2 uv 3 ����
	//1 ��Ҫ��������
	std::vector<GLfloat> positions{};
	std::vector<GLfloat> uvs{};
	std::vector<GLfloat> normals{};
	std::vector<GLuint> indices{};

	//2 ͨ������ѭ����γ�����⣬�������ڣ�->λ�á�uv
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

			float u = (float)j / (float)numLongZones; // lantitude
			float v = 1.0f - (float)i / (float)numLatBelts;  // longitude

			uvs.push_back(u);
			uvs.push_back(v);

			//ע�⣺���߷���û�����⣬���ߵĳ��Ȳ�Ϊ1
			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}
	}

	//3 ͨ������ѭ��������û��=�ţ�->��������
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

	//4 ����vbo��vao
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

	return geometry;
}

Geometry* Geometry::createPlane(float width, float height)
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
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
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

	//2 VBO����
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

	//3 EBO����
	GL_CALL(glGenBuffers(1, &geometry->mEbo));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	//4 VAO����
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

	//5.4 ����ebo����ǰ��vao
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo));

	GL_CALL(glBindVertexArray(0));

	return geometry;
}