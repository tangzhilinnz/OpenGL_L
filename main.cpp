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

//引入相机+控制器
#include "application/camera/perspectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/GameCameraControl.h"

Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

static void OnResize(int width, int height)
{
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

static void OnKey(int key, int action, int mods)
{
    cameraControl->OnKey(key, action, mods);
}

static void OnMouse(int button, int action, int mods)
{
	double x, y;
	app->getCursorPosition(&x, &y);
	cameraControl->OnMouse(button, action, x, y);
}

static void OnCursor(double xpos, double ypos)
{
	cameraControl->OnCursor(xpos, ypos);
}

void prepareCamera()
{
	float size = 10.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);
	camera = new PerspectiveCamera(
		60.0f,
		(float)app->getWidth() / (float)app->getHeight(),
		0.1f,
		1000.0f
	);

	cameraControl = new GameCameraControl();
	cameraControl->SetCamera(camera);
	cameraControl->SetSensitivity(0.4f);
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

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareCamera();

	//REND.addRenderer(std::make_unique<MipmapEX>(*camera));
	REND.addRenderer(std::make_unique<GrassLandEX>(*camera));

	REND.prepareScene();

    //执行窗体循环
    while (app->update())
    {
		cameraControl->Update();

        //渲染操作
		REND.renderAll();
    }

    app->destroy();

	return 0;
}