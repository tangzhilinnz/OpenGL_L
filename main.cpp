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
#include "renderer/readingModelEX.h"

//引入相机+控制器
#include "application/camera/perspectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/GameCameraControl.h"
#include "application/camera/trackBallCameraControl.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

glm::vec3 clearColor{};

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
	glApp->getCursorPosition(&x, &y);
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
		(float)glApp->getWidth() / (float)glApp->getHeight(),
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

static void InitIMGUI()
{
	ImGui::CreateContext();   //创建imgui上下文
	ImGui::StyleColorsDark(); // 选择一个主题

	// 设置ImGui与GLFW和OpenGL的绑定
	ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

static void RenderIMGUI() {
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
	glfwGetFramebufferSize(glApp->getWindow(), &display_w, &display_h);
	//重置视口大小
	glViewport(0, 0, display_w, display_h);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
    if (!glApp->init(800, 600))
    {
        return -1;
    }

	glApp->setResizeCallback(OnResize);
	glApp->setKeyBoardCallback(OnKey);
	glApp->setMouseCallback(OnMouse);
	glApp->setCursorCallback(OnCursor);
	glApp->setScrollCallback(OnScroll);

    //设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	PrepareCamera();
	PrepareState();
	InitIMGUI();

	//REND.addRenderer(std::make_unique<MipmapEX>(*camera));
	//REND.addRenderer(std::make_unique<GrassLandEX>(*camera));
	//REND.addRenderer(std::make_unique<CameraSystemEX>(*camera));
	//REND.addRenderer(std::make_unique<DepthTestEX>(*camera));
	//REND.addRenderer(std::make_unique<GeometryEX>(*camera));
	//REND.addRenderer(std::make_unique<StructRenderer>(*camera));
	//REND.addRenderer(std::make_unique<SpecularMask>(*camera));
	//REND.addRenderer(std::make_unique<PointLightEX>(*camera));
	//REND.addRenderer(std::make_unique<SpotLightEX>(*camera));
	//REND.addRenderer(std::make_unique<ImguiEX>(*camera));
	REND.addRenderer(std::make_unique<ReadingModelEX>(*camera));

	REND.prepareScene();

    //执行窗体循环
    while (glApp->update())
    {
		cameraControl->Update();

		REND.setClearColor(clearColor);

        //渲染操作
		REND.renderAll();
		RenderIMGUI();
    }

	REND.removeAll();

	glApp->destroy();

	delete cameraControl;
	delete camera;
	return 0;
}