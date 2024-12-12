#include <iostream>

// Note: glad.h must be in front of glfw3.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"


//������ʵ��һ����Ӧ�����С�仯�ĺ���
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    //	std::cout << "�������´�С��" << width << " , " << height << std::endl;
    glViewport(0, 0, width, height);
}

//������ʵ��һ��������Ϣ�ص�����
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W) {
        //������w
    }
    if (action == GLFW_PRESS) {
        //��λ����
    }
    if (action == GLFW_RELEASE) {
        //��λ̧��
    }
    if (mods == GLFW_MOD_CONTROL) {
        //����ctrl��ͬʱ��������key
    }
    if (mods == GLFW_MOD_SHIFT) {
        //����shift��ͬʱ��������key
    }
    std::cout << "�����ˣ�" << key << std::endl;
    std::cout << "action��" << action << std::endl;
    std::cout << "mods��" << mods << std::endl;
}

int main()
{
    glfwInit();
    // Sets the major version of the OpenGL context you want to create 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // Sets the minor version of the OpenGL context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Sets the profile for the OpenGL context to Core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the windows
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Core", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //���õ�ǰ�������ΪOpenGL�Ļ�����̨
    glfwMakeContextCurrent(window);

    //���ü��������������С�仯����Ϣ
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    //���ü���������������Ϣ
    glfwSetKeyCallback(window, keyCallBack);

    //ʹ��glad�������е�ǰ�汾opengl�ĺ���
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //����opengl�ӿ��Լ�������ɫ
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    //ִ�д���ѭ��
    while (!glfwWindowShouldClose(window))
    {
        //���ղ��ַ�������Ϣ
        //�����Ϣ�����Ƿ�����Ҫ�������ꡢ���̵���Ϣ
        //����еĻ��ͽ���Ϣ����������ն���
        glfwPollEvents();

        //ִ��opengl�����������
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        //��Ⱦ����

        //�л�˫����
        glfwSwapBuffers(window);
    }

    glfwTerminate();

	return 0;
}