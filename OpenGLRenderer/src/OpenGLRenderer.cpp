#include <GL/OpenGLHelper.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Logger/Logger.h>
#include <Shader/ShaderManager.h>
#include <Component/Camera/Camera.h>
#include <Action.h>
#include <Scene/Scene.h>
#include <Scene/Scenes/ColorScene.h>
#include <Scene/Scenes/TextureScene.h>
#include <Window/WindowManager.h>
#include <GUI/Scenes/ScenesWindow.h>
#include <GUI/Inspector/InspectorWindow.h>
#include <GUI/Hierarchy/HeirarchyWindow.h>
#include <Scene/Scenes/ColorScene.h>
#include <Scene/Scenes/TextureScene.h>
#include <Scene/Scenes/ModelScene.h>

using namespace core;
using namespace core::gl;

int main(void)
{
	if (!OpenGLHelper::InitializeGLFW())
		return -1;

	ShaderManager* shaderManager = ShaderManager::getInstance();
	WindowManager* windowManager = WindowManager::getInstance();

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

	Window* window1 = windowManager->GetWindowInstance("Main Window", 2000, 1000, window, globalFontAtlas);
	Window* window2 = windowManager->GetWindowInstance("Second Window", 960, 480, window, globalFontAtlas);

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
		scenesWindow.RegisterScene<scene::ModelScene>("ModelScene");

		window1->RegisterGUIWindow(scenesWindow);
		window2->RegisterGUIWindow(scenesWindow);

		InspectorWindow inspectorWindow;

		window1->RegisterGUIWindow(inspectorWindow);
		window2->RegisterGUIWindow(inspectorWindow);

		HeirarchyWindow heirarchyWindow;

		window1->RegisterGUIWindow(heirarchyWindow);
		window2->RegisterGUIWindow(heirarchyWindow);

		windowManager->Init();
		windowManager->WindowLoop();
		windowManager->Exit();
	}

	delete window1;
	delete window2;

	OpenGLHelper::TerminateGLFW();

	return 0;
}