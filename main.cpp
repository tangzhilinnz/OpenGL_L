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

//�������+������
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

// ����
static void OnKey(int key, int action, int mods)
{
    cameraControl->OnKey(key, action, mods);
}

//��갴��/̧��
static void OnMouse(int button, int action, int mods)
{
	double x, y;
	glApp->getCursorPosition(&x, &y);
	cameraControl->OnMouse(button, action, x, y);
}

//����ƶ�
static void OnCursor(double xpos, double ypos)
{
	cameraControl->OnCursor(xpos, ypos);
}

//������
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
	ImGui::CreateContext();   //����imgui������
	ImGui::StyleColorsDark(); // ѡ��һ������

	// ����ImGui��GLFW��OpenGL�İ�
	ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

static void RenderIMGUI() {
	//1 ������ǰ��IMGUI��Ⱦ
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//2 ������ǰ��GUI��������Щ�ؼ������ϵ���
	ImGui::Begin("Hello, world!");
	ImGui::Text("ChangeColor Demo");
	ImGui::Button("Test Button", ImVec2(40, 20));
	ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
	ImGui::End();

	//3 ִ��UI��Ⱦ
	ImGui::Render();
	//��ȡ��ǰ����Ŀ��
	int display_w, display_h;
	glfwGetFramebufferSize(glApp->getWindow(), &display_w, &display_h);
	//�����ӿڴ�С
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

    //����opengl�ӿ��Լ�������ɫ
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

    //ִ�д���ѭ��
    while (glApp->update())
    {
		cameraControl->Update();

		REND.setClearColor(clearColor);

        //��Ⱦ����
		REND.renderAll();
		RenderIMGUI();
    }

	REND.removeAll();

	glApp->destroy();

	delete cameraControl;
	delete camera;
	return 0;
}