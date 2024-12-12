#include <iostream>

// Note: glad.h must be in front of glfw3.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"
#include "application/Application.h"


void OnResize(int width, int height)
{
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods)
{
    std::cout << key << std::endl;
}

int main()
{
    if (!app->init(800, 600))
    {
        return -1;
    }

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallback(OnKey);

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    //执行窗体循环
    while (app->update())
    {
        //执行opengl画布清理操作
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        //渲染操作
    }

    app->destroy();

	return 0;
}