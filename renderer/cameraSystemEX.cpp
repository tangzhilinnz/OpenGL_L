#include "cameraSystemEx.h"
#include "../wrapper/checkError.h"

CameraSystemEX::CameraSystemEX(const Camera& _rCamera)
	: rCamera(_rCamera)
{}

void CameraSystemEX::prepareVAO()
{
	float positions[] = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	float colors[] = {
		1.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 0.0f,1.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
	};

	//2 VBO����
	GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO����
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO����
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	//5 ��vbo ebo ��������������Ϣ
	//5.1 ����λ������������Ϣ
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.2 ������ɫ������������
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.3 ����uv������������
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 ����ebo����ǰ��mVao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void CameraSystemEX::prepareShader()
{
	mShader.initShader("assets/shaders/CameraV.glsl", "assets/shaders/CameraF.glsl");
}

void CameraSystemEX::prepareTexture()
{
	mTexture.initTexture("assets/textures/goku.jpg", 0, true);
}

void CameraSystemEX::render()
{
	//ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//�󶨵�ǰ��program
	mShader.begin();
	mShader.setInt("sampler", 0);
	mShader.setMatrix4x4("transform", transform);
	mShader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
	mShader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

	//�󶨵�ǰ��vao
	GL_CALL(glBindVertexArray(mVao));

	mTexture.bind();
	//��������ָ��
	GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));

	mShader.end();
}