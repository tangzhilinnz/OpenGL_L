#include <iostream>
#include <thread> // For sleep_for

// Note: glad.h must be in front of glfw3.h
#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"
#include "glframework/framebuffer.h"

#include "renderer/rendererManager.h"
//#include "renderer/mipmapEX.h"
//#include "renderer/grasslandEX.h"
//#include "renderer/cameraSystemEX.h"
//#include "renderer/depthTestEX.h"
//#include "renderer/geometryEX.h"
//#include "renderer/simpleLightEX.h"
//#include "renderer/structRenderer.h"
//#include "renderer/specularMask.h"
//#include "renderer/pointLightEX.h"
//#include "renderer/spotLightEX.h"
//#include "renderer/imguiEX.h"
//#include "renderer/readingModelEX.h"
#include "renderer/depthTest2EX.h"
#include "renderer/blendOITEX.h"

//引入相机+控制器
#include "application/camera/perspectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/GameCameraControl.h"
#include "application/camera/trackBallCameraControl.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
struct LeakDetector
{
	LeakDetector()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
};

LeakDetector leakDetector;
#endif

Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

glm::vec3 clearColor{};

static void OnResize(int width, int height)
{
	std::cout << width << "   " << height << std::endl;
	
	if (width == 0 || height == 0)
	{
		return;
	}

	AttachmentGL::resizeAllInstances(width, height);
	FboGL::reBindAllInstances();
    GL_CALL(glViewport(0, 0, width, height));
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
	glApp.getCursorPosition(&x, &y);
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
		(float)glApp.getWidth() / (float)glApp.getHeight(),
		0.1f,
		500.0f
	);

	camera->SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));
	cameraControl = new GameCameraControl();
	//cameraControl = new TrackBallCameraControl();
	cameraControl->SetCamera(camera);
	cameraControl->SetSensitivity(0.5f);
	cameraControl->SetMoveSpeed(0.08f);
}

static void InitIMGUI()
{
	ImGui::CreateContext();   //创建imgui上下文
	ImGui::StyleColorsDark(); // 选择一个主题

	// 设置ImGui与GLFW和OpenGL的绑定
	ImGui_ImplGlfw_InitForOpenGL(glApp.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

static void RenderIMGUI()
{
	//1 开启当前的IMGUI渲染
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//2 决定当前的GUI上面有哪些控件，从上到下
	ImGui::Begin("Hello, world!");
	ImGui::Text("ChangeColor Demo");
	ImGui::Button("Test Button", ImVec2(40, 20));
	ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
	ImGui::End();

	//3 执行UI渲染
	ImGui::Render();
	//获取当前窗体的宽高
	int display_w, display_h;
	glfwGetFramebufferSize(glApp.getWindow(), &display_w, &display_h);
	//重置视口大小
	glViewport(0, 0, display_w, display_h);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void CleanupIMGUI()
{
	// Clean up ImGui resources
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	// Destroy the ImGui context and free memory
	ImGui::DestroyContext();
}

int main()
{
    if (!glApp.init(800, 600))
    {
        return -1;
    }

	glApp.setResizeCallback(OnResize);
	glApp.setKeyBoardCallback(OnKey);
	glApp.setMouseCallback(OnMouse);
	glApp.setCursorCallback(OnCursor);
	glApp.setScrollCallback(OnScroll);

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	PrepareCamera();
	PrepareState();
	InitIMGUI();

	glApp.GPUInfo();

	//REND.setRenderer(std::make_unique<MipmapEX>(*camera));
	//REND.setRenderer(std::make_unique<GrassLandEX>(*camera));
	//REND.setRenderer(std::make_unique<CameraSystemEX>(*camera));
	//REND.setRenderer(std::make_unique<DepthTestEX>(*camera));
	//REND.setRenderer(std::make_unique<GeometryEX>(*camera));
	//REND.setRenderer(std::make_unique<SimpleLightEX>(*camera));
	//REND.setRenderer(std::make_unique<StructRenderer>(*camera));
	//REND.setRenderer(std::make_unique<SpecularMask>(*camera));
	//REND.setRenderer(std::make_unique<PointLightEX>(*camera));
	//REND.setRenderer(std::make_unique<SpotLightEX>(*camera));
	//REND.setRenderer(std::make_unique<ImguiEX>(*camera));
	//REND.setRenderer(std::make_unique<ReadingModelEX>(*camera));
	//REND.setRenderer(std::make_unique<DepthTest2EX>(*camera));
	REND.setRenderer(std::make_unique<BlendOITEX>(*camera));

	REND.prepareScene();

    //执行窗体循环
    while (glApp.update())
    {
		if (glApp.isWindowMinimized())
		{
			std::cout << "Window is minimized. Pausing rendering..." << std::endl;

			// Optionally sleep to save resources
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			// Skip rendering and updates
			continue;
		}

		cameraControl->Update();

		REND.setClearColor(clearColor);

        //渲染操作
		REND.render();
		RenderIMGUI();
    }

	REND.clearRenderer();

	glApp.destroy();

	delete cameraControl;
	delete camera;

	CleanupIMGUI();

	return 0;
}