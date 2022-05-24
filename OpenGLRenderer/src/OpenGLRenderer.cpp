#include "Core/GL/OpenGLHelper.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Macros.h"
#include "Core/Logger.h"
#include "Core/GL/Texture/Texture.h"
#include "Systems/Shader/Shader.h"
#include "Systems/Shader/ShaderManager.h"
#include "Systems/GameVastu/GameVastuManager.h"
#include "Systems/Component/Camera/Camera.h"
#include "Systems/Window/WindowManager.h"
#include "Utils/Action.h"
#include "Systems/GUI/ScenesWindow.h"
#include "Systems/Scene/Scenes/ColorScene.h"
#include "Systems/Scene/Scenes/TextureScene.h"

int main(void)
{
	if (!OpenGLHelper::InitializeGLFW())
		return -1;

	ShaderManager* shaderManager = ShaderManager::getInstance();
	WindowManager* windowManager = WindowManager::getInstance();
	GameVastuManager* gameVastuManager = GameVastuManager::getInstance();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = OpenGLHelper::CreateWindow(1, 1, "", NULL, true);
	OpenGLHelper::UseGLFWWindow(window);
	OpenGLHelper::SetSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImFontAtlas* globalFontAtlas = ImGui::GetIO().Fonts;

	if (!OpenGLHelper::InitializeGLEW())
		return -1;

	Window* window1 = windowManager->GetWindowInstance("Main Window", 1280, 960, window, globalFontAtlas);
	Window* window2 = windowManager->GetWindowInstance("Second Window", 640, 480, window, globalFontAtlas);

	fprintf(stdout, "Status: Using OpenGL version %s\n", glGetString(GL_VERSION));

	{
		// Vertex		|	
		// Attrib Ptr	|	Vertex Buffer Layouts
		// -------------+------------------------------
		//				|	Vertex		|	Tex Coords
		// -------------+---------------+--------------
		// 0			|	-0.5, -0.5, |	0.0f, 0.0f,		
		// 1			|	 0.5, -0.5, |	1.0f, 0.0f,		
		// 2			|	 0.5,  0.5,	|	1.0f, 1.0f,		
		// 3			|	-0.5,  0.5, |	0.0f, 1.0f		

		ShaderAsset shaderAsset1 = shaderManager->CreateShaderAsset("resources/shaders/Texture.shader");
		ShaderAsset shaderAsset2 = shaderManager->CreateShaderAsset("resources/shaders/Color.shader");

		ScenesWindow scenesWindow;
		scenesWindow.RegisterScene<scene::ColorScene>("ColorScene");
		scenesWindow.RegisterScene<scene::TextureScene>("TextureScene");

		window1->RegisterGUIWindow(scenesWindow);
		window2->RegisterGUIWindow(scenesWindow);

		// Register Ordering Matters
		//window1->RegisterGameVastu(*colorVastu);
		//window1->RegisterGameVastu(*textureVastu);
		//window2->RegisterGameVastu(*colorVastu);
		//window2->RegisterGameVastu(*textureVastu);

		bool show_demo_window = false;
		bool show_another_window = true;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		std::function<void(const Window&)> GUIUpdateAction = [](const Window& window)
		{
			Camera& camera = window.GetCamera();
			GameVastu* cameraVastu = camera.GetGameVastu();
			Transform* cameraTransform = cameraVastu->m_transform;
			ImGui::Begin("Camera");

			ImGui::Text("Id:");
			std::string id = std::to_string(cameraVastu->GetId());
			ImGui::Text(id.c_str());

			ImGui::Separator();

			ImGui::Text("Tranform:");
			glm::vec3 campos = cameraTransform->GetPosition();
			float* camposArr = glm::value_ptr(campos);
			if (ImGui::DragFloat3("P", camposArr))
				cameraTransform->SetPosition(glm::make_vec3(camposArr));
			glm::vec3 camrot = cameraTransform->GetRotation();
			float* camrotArr = glm::value_ptr(camrot);
			if (ImGui::DragFloat3("R", camrotArr))
				cameraTransform->SetRotation(glm::make_vec3(camrotArr));
			glm::vec3 camscale = cameraTransform->GetScale();
			float* camscaleArr = glm::value_ptr(camscale);
			if (ImGui::DragFloat3("S", camscaleArr))
				cameraTransform->SetScale(glm::make_vec3(camscaleArr));

			ImGui::Separator();

			ImGui::Text("Clip Planes:");
			float nearClipPlane = camera.GetNearClipPlane();
			if (ImGui::DragFloat("Near", &nearClipPlane))
				camera.SetNearClipPlane(nearClipPlane);
			float farClipPlane = camera.GetFarClipPlane();
			if (ImGui::DragFloat("Far", &farClipPlane))
				camera.SetNearClipPlane(farClipPlane);

			ImGui::End();
		};

		windowManager->UpdateActionsRenderGUI.push_back(GUIUpdateAction);

		windowManager->Init();
		windowManager->WindowLoop();
		windowManager->Exit();
	}

	delete window1;
	delete window2;

	OpenGLHelper::TerminateGLFW();

	return 0;
}