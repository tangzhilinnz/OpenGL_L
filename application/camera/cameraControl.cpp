#include "cameraControl.h"
#include <iostream>

void CameraControl::OnMouse(int button, int action, double xpos, double ypos)
{
	//1 判断当前的按键，是否按下
	bool pressed = (action == GLFW_PRESS ? true : false);

	//2 如果按下，记录当前按下的位置
	if (pressed)
	{
		mCurrentX = (float)xpos;
		mCurrentY = (float)ypos;
	}

	//3 根据按下的鼠标按键不同，更改按键状态
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	default:
		std::cerr << "Warning: Unsupported mouse button (" 
			      << button << ") detected.\n";
		break;
	}

}

void CameraControl::OnCursor(double xpos, double ypos)
{}

void CameraControl::OnKey(int key, int action, int mods)
{
	//过滤掉repeat的情况
	if (action == GLFW_PRESS)
	{
		return;
	}

	//1 检测按下或者抬起，给到一个变量
	bool pressed = (action == GLFW_PRESS ? true : false);

	//2 记录在keyMap
	mKeyMap[key] = pressed;
}

void CameraControl::Update()
{}