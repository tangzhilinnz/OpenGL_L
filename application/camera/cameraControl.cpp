#include "cameraControl.h"
#include <iostream>

void CameraControl::OnMouse(int button, int action, double xpos, double ypos)
{
	//1 �жϵ�ǰ�İ������Ƿ���
	bool pressed = (action == GLFW_PRESS ? true : false);

	//2 ������£���¼��ǰ���µ�λ��
	if (pressed)
	{
		mCurrentX = (float)xpos;
		mCurrentY = (float)ypos;
	}

	//3 ���ݰ��µ���갴����ͬ�����İ���״̬
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
	//���˵�repeat�����
	if (action == GLFW_PRESS)
	{
		return;
	}

	//1 ��ⰴ�»���̧�𣬸���һ������
	bool pressed = (action == GLFW_PRESS ? true : false);

	//2 ��¼��keyMap
	mKeyMap[key] = pressed;
}

void CameraControl::Update()
{}