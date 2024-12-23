#pragma once

#include "../../glframework/core.h"
#include "camera.h"

#include <map>

class CameraControl
{
public:
	CameraControl() = default;
	CameraControl(const CameraControl&) = default;
	CameraControl& operator=(const CameraControl&) = default;
	~CameraControl() = default;

	//���ڼ̳�CameraControl�����࣬�п��ܻ�ʵ���Լ����߼�
	virtual void OnMouse(int button, int action, double xpos, double ypos);
	virtual void OnCursor(double xpos, double ypos);
	virtual void OnKey(int key, int action, int mods);
	virtual void OnScroll(double offset); //+1 -1
	
	//ÿһ֡��Ⱦ֮ǰ��Ҫ���е��ã�ÿһ֡���µ���Ϊ���Է�������
	virtual void Update();

	void SetCamera(Camera* camera) { mCamera = camera; }
	void SetSensitivity(float s) { mSensitivity = s; }
	void SetScaleSpeed(float s) { mScaleSpeed = s; }
	void SetMoveSpeed(float s) { mMoveSpeed = s; }

	Camera* GetCameraPtr() const { return mCamera; }

protected:
	//1 ��갴��״̬
	bool mLeftMouseDown = false;
	bool mRightMouseDown = false;
	bool mMiddleMouseDown = false;

	//2 ��ǰ����λ��
	float mCurrentX = 0.0f;
    float mCurrentY = 0.0f;

	//3 ���ж�
	float mSensitivity = 0.2f;

	//4 ��¼������ذ����İ���״̬
	std::map<int, bool> mKeyMap;

	//5 �洢��ǰ���Ƶ���һ�������
	Camera* mCamera = nullptr;

	//6 ��¼������ŵ��ٶ�
	float mScaleSpeed = 0.2f;

	float mMoveSpeed = 0.2f;
};