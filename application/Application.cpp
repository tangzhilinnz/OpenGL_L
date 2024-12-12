#include "Application.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

//��ʼ��Application�ľ�̬����
Application* Application::mInstance = nullptr;

Application* Application::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}

	return mInstance;
}

Application::~Application()
{
	delete mInstance;
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
	glfwSetKeyCallback(mWindow, Application::keyCallback);

	// this���ǵ�ǰȫ��Ψһ��Application����
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
	//�����Ϣ�����Ƿ�����Ҫ��������ꡢ���̵���Ϣ
	//����еĻ��ͽ���Ϣ������������ն���
	glfwPollEvents();

	//�л�˫����
	glfwSwapBuffers(mWindow);

	return true;
}

void Application::destroy()
{
	//�˳�����ǰ���������
	glfwTerminate();
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "Resize" << std::endl;

	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->mResizeCallback != nullptr)
	{
		self->mResizeCallback(width, height);
	}
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);

	if (self->mKeyBoardCallback != nullptr)
	{
		self->mKeyBoardCallback(key, action, mods);
	}
}