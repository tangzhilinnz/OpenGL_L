#include <iostream>

// Note: glad.h must be in front of glfw3.h
#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"
#include "application/Application.h"

GLuint vao;
Shader shader;

void OnResize(int width, int height)
{
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods)
{
    std::cout << key << std::endl;
}

void prepareVAO()
{
	//1 ׼��positions
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};

	float colors[] = {
		1.f,  0.f, 0.f,
		0.f,  1.f, 0.f,
		0.f,  0.f, 1.f,
	};

	unsigned int indices[] = {
		0, 1, 2,
	};

	//2 VBO����
	GLuint posVbo, colorVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), colors, GL_STATIC_DRAW);

	//3 EBO����
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO����
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//5 ��vbo ebo ��������������Ϣ
	//5.1 ����λ������������Ϣ
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.2 ����ebo����ǰ��vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void prepareShader()
{
	shader.initShader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

void render()
{
	//ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//1 �󶨵�ǰ��program
	shader.begin();
	    shader.setFloat("time", (float)glfwGetTime());
	    
		//2 �󶨵�ǰ��vao
		glBindVertexArray(vao);

		//3 ��������ָ��
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
		//glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		//glDrawArrays(GL_LINES, 0, 6);
		//glDrawArrays(GL_LINE_STRIP, 0, 6);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	
		glBindVertexArray(0);
	shader.end();
}

int main()
{
    if (!app->init(800, 600))
    {
        return -1;
    }

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallback(OnKey);

    //����opengl�ӿ��Լ�������ɫ
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareShader();
	prepareVAO();

    //ִ�д���ѭ��
    while (app->update())
    {
        //��Ⱦ����
		render();
    }

    app->destroy();

	return 0;
}