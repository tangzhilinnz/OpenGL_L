#include "grasslandEX.h"
#include "../wrapper/checkError.h"

GrassLandEX::GrassLandEX(const Camera& _rCamera)
	: rCamera(_rCamera)
{}

void GrassLandEX::prepareVAO()
{
	//1 准备positions
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	float colors[] = {
		1.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 0.0f,1.0f,
		0.5f, 0.5f,0.5f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
	};

	//2 VBO创建
	GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO创建
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	//5 绑定vbo ebo 加入属性描述信息
	//5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.2 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void GrassLandEX::prepareShader()
{
	mShader.initShader("assets/shaders/GrassLandV.glsl", "assets/shaders/GrassLandF.glsl");
}

void GrassLandEX::prepareTexture()
{
	mGrassTexture.initTexture("assets/textures/grass.jpg", 0);
	mLandTexture.initTexture("assets/textures/land.jpg", 1);
	mNoiseTexture.initTexture("assets/textures/noise.jpg", 2);
}

void GrassLandEX::render()
{
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//1 绑定当前的program
	mShader.begin();
	mShader.setInt("grassSampler", 0);
	mShader.setInt("landSampler", 1);
	mShader.setInt("noiseSampler", 2);

	mShader.setMatrix4x4("transform", transform);
	mShader.setMatrix4x4("viewMatrix", rCamera.GetViewMatrix());
	mShader.setMatrix4x4("projectionMatrix", rCamera.GetProjectionMatrix());

	//2 绑定当前的vao
	glBindVertexArray(mVao);

	//3 发出绘制指令
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	//glDrawArrays(GL_LINES, 0, 6);
	//glDrawArrays(GL_LINE_STRIP, 0, 6);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	mShader.end();
}