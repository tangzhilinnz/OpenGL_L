#include "Application.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <assert.h>

Application& Application::getInstance()
{
	static Application mInstance;
	return mInstance;
}

Application::~Application()
{
	//delete mInstance;
}

bool Application::init(const int& width, const int& height)
{
	mWidth = width;
	mHeight = height;

	//1 初始化GLFW基本环境
	glfwInit();

	//1.1 设置OpenGL主版本号、次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//1.2 设置OpenGL启用核心模式（非立即渲染模式）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//2 创建窗体对象
	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL_L", NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	//设置当前窗体对象为OpenGL的绘制舞台
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(mWindow, Application::frameBufferSizeCallback);

	//键盘响应
	glfwSetKeyCallback(mWindow, Application::keyCallback);

	//鼠标点击事件响应
	glfwSetMouseButtonCallback(mWindow, Application::mouseCallback);

	//鼠标移动事件响应
	glfwSetCursorPosCallback(mWindow, Application::cursorCallback);

	//鼠标滚轮消息
	glfwSetScrollCallback(mWindow, Application::scrollCallback);

	// Each GLFW window has a unique "user pointer" slot, which can hold a
	// single void*
	// You can use it to associate any kind of data
	// (e.g., application state,custom objects) with the window.
	glfwSetWindowUserPointer(mWindow, this);

	return true;
}

bool Application::update()
{
	if (glfwWindowShouldClose(mWindow))
	{
		return false;
	}

	//接收并分发窗体消息
	//检查消息队列是否有需要处理的鼠标、键盘等消息
	//如果有的话就将消息批量处理，清空队列
	glfwPollEvents();

	//切换双缓存
	glfwSwapBuffers(mWindow);

	return true;
}

void Application::destroy()
{
	// Destroy the GLFW window
	if (mWindow != nullptr)
	{
		glfwDestroyWindow(mWindow);
		mWindow = nullptr;
	}

	// Terminate GLFW
	glfwTerminate();
}

void Application::getCursorPosition(double* x, double* y) const
{
	assert(x && y);
	glfwGetCursorPos(mWindow, x, y);
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "Resize" << std::endl;

	// Retrieve the Application instance
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	// Retrieve the Application instance
	if (self)
	{
		self->mWidth = width;
		self->mHeight = height;

		if (self->mResizeCallback != nullptr)
		{
			self->mResizeCallback(width, height);
		}
	}
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode,
	                          int action, int mods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->mKeyBoardCallback != nullptr)
	{
		self->mKeyBoardCallback(key, action, mods);
	}
}

void Application::mouseCallback(GLFWwindow* window, int button, int action,
	                            int mods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->mMouseCallback != nullptr)
	{
		self->mMouseCallback(button, action, mods);
	}
}

void Application::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->mCursorCallback != nullptr)
	{
		self->mCursorCallback(xpos, ypos);
	}
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->mScrollCallback != nullptr)
	{
		self->mScrollCallback(yoffset);
	}
}