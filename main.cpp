#include <iostream>

// Note: glad.h must be in front of glfw3.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"


//声明且实现一个响应窗体大小变化的函数
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    //	std::cout << "窗体最新大小：" << width << " , " << height << std::endl;
    glViewport(0, 0, width, height);
}

//声明且实现一个键盘消息回调函数
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W) {
        //触发了w
    }
    if (action == GLFW_PRESS) {
        //键位按下
    }
    if (action == GLFW_RELEASE) {
        //键位抬起
    }
    if (mods == GLFW_MOD_CONTROL) {
        //按下ctrl的同时，按下了key
    }
    if (mods == GLFW_MOD_SHIFT) {
        //按下shift的同时，按下了key
    }
    std::cout << "按下了：" << key << std::endl;
    std::cout << "action：" << action << std::endl;
    std::cout << "mods：" << mods << std::endl;
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
    //设置当前窗体对象为OpenGL的绘制舞台
    glfwMakeContextCurrent(window);

    //设置监听，监听窗体大小变化的消息
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    //设置监听，监听键盘消息
    glfwSetKeyCallback(window, keyCallBack);

    //使用glad加载所有当前版本opengl的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    //执行窗体循环
    while (!glfwWindowShouldClose(window))
    {
        //接收并分发窗体消息
        //检查消息队列是否有需要处理的鼠标、键盘等消息
        //如果有的话就将消息批量处理，清空队列
        glfwPollEvents();

        //执行opengl画布清理操作
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        //渲染操作

        //切换双缓存
        glfwSwapBuffers(window);
    }

    glfwTerminate();

	return 0;
}