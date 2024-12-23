#pragma once

#include "cameraControl.h"

class TrackBallCameraControl : public CameraControl
{
public:
	TrackBallCameraControl() = default;
	TrackBallCameraControl(const TrackBallCameraControl&) = default;
	TrackBallCameraControl& operator=(const TrackBallCameraControl&) = default;
	~TrackBallCameraControl() = default;

	void OnCursor(double xpos, double ypos) override;

	void OnScroll(double offset) override;

private:
	void Pitch(float angle);
	void Yaw(float angle);

private:
	float mSpeed{ 0.1f };
	float mTotalPitch{ 0.0f };
};