#include <iostream>

// Note: glad.h must be in front of glfw3.h
#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"

#include "renderer/rendererManager.h"
#include "renderer/mipmapEX.h"
#include "renderer/grasslandEX.h"
#include "renderer/cameraSystemEX.h"
#include "renderer/depthTestEX.h"
#include "renderer/geometryEX.h"
#include "renderer/simpleLightEX.h"

//引入相机+控制器
#include "application/camera/perspectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/GameCameraControl.h"
#include "application/camera/trackBallCameraControl.h"

Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

static void OnResize(int width, int height)
{
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

// 键盘
static void OnKey(int key, int action, int mods)
{
    cameraControl->OnKey(key, action, mods);
}

//鼠标按下/抬起
static void OnMouse(int button, int action, int mods)
{
	double x, y;
	app->getCursorPosition(&x, &y);
	cameraControl->OnMouse(button, action, x, y);
}

//鼠标移动
static void OnCursor(double xpos, double ypos)
{
	cameraControl->OnCursor(xpos, ypos);
}

//鼠标滚轮
static void OnScroll(double offset)
{
	cameraControl->OnScroll(offset);
}



static void PrepareState()
{
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	// glClearDepth(0.0f);
}

static void PrepareCamera()
{
	//float size = 10.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);
	camera = new PerspectiveCamera(
		60.0f,
		(float)app->getWidth() / (float)app->getHeight(),
		0.1f,
		1000.0f
	);

	camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));

	cameraControl = new GameCameraControl();
	//cameraControl = new TrackBallCameraControl();
	cameraControl->SetCamera(camera);
	cameraControl->SetSensitivity(0.5f);
	cameraControl->SetMoveSpeed(0.02f);
}

int main()
{
    if (!app->init(800, 600))
    {
        return -1;
    }

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallback(OnKey);
	app->setMouseCallback(OnMouse);
	app->setCursorCallback(OnCursor);
	app->setScrollCallback(OnScroll);

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	PrepareCamera();
	PrepareState();

	//REND.addRenderer(std::make_unique<MipmapEX>(*camera));
	//REND.addRenderer(std::make_unique<GrassLandEX>(*camera));
	//REND.addRenderer(std::make_unique<CameraSystemEX>(*camera));
	//REND.addRenderer(std::make_unique<DepthTestEX>(*camera));
	//REND.addRenderer(std::make_unique<GeometryEX>(*camera));
	REND.addRenderer(std::make_unique<SimpleLightEX>(*camera));

	REND.prepareScene();

    //执行窗体循环
    while (app->update())
    {
		cameraControl->Update();

        //渲染操作
		REND.renderAll();
    }

	REND.removeAll();

    app->destroy();

	delete cameraControl;
	delete camera;
	return 0;
}