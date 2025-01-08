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

	//1 ��ʼ��GLFW��������
	glfwInit();

	//1.1 ����OpenGL���汾�š��ΰ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//1.2 ����OpenGL���ú���ģʽ����������Ⱦģʽ��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//2 �����������
	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL_L", NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	//���õ�ǰ�������ΪOpenGL�Ļ�����̨
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(mWindow, Application::frameBufferSizeCallback);

	//������Ӧ
	glfwSetKeyCallback(mWindow, Application::keyCallback);

	//������¼���Ӧ
	glfwSetMouseButtonCallback(mWindow, Application::mouseCallback);

	//����ƶ��¼���Ӧ
	glfwSetCursorPosCallback(mWindow, Application::cursorCallback);

	//��������Ϣ
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

	//���ղ��ַ�������Ϣ
	//�����Ϣ�����Ƿ�����Ҫ�������ꡢ���̵���Ϣ
	//����еĻ��ͽ���Ϣ����������ն���
	glfwPollEvents();

	//�л�˫����
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