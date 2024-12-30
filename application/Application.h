#pragma once 

/*
*����������������������������������������������������������������������������������������������������
*����Ŀ	   �꣺ ��װApplication����ʾ�˵�ǰӦ�ó�����
*������    ʦ�� ������(Carma Zhao)
*�������Ŀ�꣺
*��
*����		1	�����ࣨȫ��Ψһʵ����
*����		2	��Ա���� + ��Ա����
*				2.1 ��Ա����-init����ʼ����
*				2.2 ��Ա����-update��ÿһִ֡�У�
*				2.3 ��Ա����-destroy����βִ�У�
*����		3	��Ӧ�ص�����(Resize)
*				3.1 ����һ������ָ��ResizeCallback
*				3.2 ����һ��ResizeCallback���͵ĳ�Ա����
*				3.3 ����һ��SetResizeCallback�ĺ��� �����ô���仯��Ӧ�ص�����
*				3.4 ����һ��static�ľ�̬������������Ӧglfw����仯
*				3.5 ����̬�������õ�glfw�ļ���Resize��������
*				3.6 * ѧ��ʹ��glfw��UserPointer
*����		4	��Ӧ������Ϣ����(KeyBoard)
*				3.1 ����һ��static�ľ�̬������������Ӧglfw�ļ����¼�
*				3.2 ����̬�������õ�glfw�ļ���KeyCallback��������
*				3.3 ����һ������ָ��KeyBoardCallback
*				3.4 ����һ��KeyBoardCallback���͵ĳ�Ա����
*				3.5 ����һ��SetKeyBoardCallback�ĺ��� �����ü�����Ӧ�ص�����
*				3.6 * ѧ��ʹ��glfw��UserPointer
*����������������������������������������������������������������������������������������������������
*/

#include <iostream>

#define app Application::getInstance()

struct GLFWwindow;

using ResizeCallback = void(*)(int width, int height);
using KeyBoardCallback = void(*)(int key, int action, int mods);
using MouseCallback = void(*)(int button, int action, int mods);
using CursorCallback = void(*)(double xpos, double ypos);
using ScrollCallback = void(*)(double offset);

class Application
{
public:
	~Application();

	//���ڷ���ʵ���ľ�̬����
	static Application* getInstance();

	bool init(const int& width = 800, const int& height = 600);
	bool update();
	void destroy();

	GLFWwindow* getWindow()const { return mWindow; }

	uint32_t getWidth() const { return mWidth; }
	uint32_t getHeight() const { return mHeight; }
	void getCursorPosition(double* x, double* y) const;

	void setResizeCallback(ResizeCallback callback)     { mResizeCallback = callback; }
	void setKeyBoardCallback(KeyBoardCallback callback) { mKeyBoardCallback = callback; }
	void setMouseCallback(KeyBoardCallback callback)    { mMouseCallback = callback; }
	void setCursorCallback(CursorCallback callback)     { mCursorCallback = callback; }
	void setScrollCallback(ScrollCallback callback)     { mScrollCallback = callback; }

private:
	//C++���ں���ָ��
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	static Application* mInstance;

	uint32_t mWidth{0};
	uint32_t mHeight{0};

	GLFWwindow* mWindow{nullptr};

	ResizeCallback mResizeCallback     { nullptr };
	KeyBoardCallback mKeyBoardCallback { nullptr };
	MouseCallback mMouseCallback       { nullptr };
	CursorCallback mCursorCallback     { nullptr };
	ScrollCallback mScrollCallback     { nullptr };

	Application() = default;
};