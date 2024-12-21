#pragma once

#include "cameraControl.h"

class GameCameraControl : public CameraControl
{
public:
	GameCameraControl() = default;
	GameCameraControl(const GameCameraControl&) = default;
	GameCameraControl& operator=(const GameCameraControl&) = default;
	~GameCameraControl() = default;

	void OnCursor(double xpos, double ypos)override;
	void Update()override;

private:
	void Pitch(float angle);
	void Yaw(float angle);
private:

	float mPitch{ 0.0f };
};