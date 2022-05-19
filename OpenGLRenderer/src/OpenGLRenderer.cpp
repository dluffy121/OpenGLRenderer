#include "Core/GL/OpenGLHelper.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/gtx/string_cast.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Macros.h"
#include "Core/Logger.h"
#include "Core/GL/Texture/Texture.h"
#include "Systems/Shader/Shader.h"
#include "Systems/Shader/ShaderManager.h"
#include "Systems/Window/WindowManager.h"
#include "Systems/GameVastu/GameVastuManager.h"
#include "Systems/Component/Camera/Camera.h"
#include "Systems/Component/Renderer/Renderer.h"
#include "Utils/Action.h"
#include <glm/gtc/type_ptr.hpp>

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
	GLFWwindow* window = OpenGLHelper::CreateWindow(640, 480, "", NULL, true);
	OpenGLHelper::UseGLFWWindow(window);
	OpenGLHelper::SetSwqpInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImFontAtlas* globalFontAtlas = ImGui::GetIO().Fonts;

	GLLog(glEnable(GL_BLEND));
	GLLog(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Window* window1 = windowManager->GetWindowInstance("Main Window", 1280, 960, window, globalFontAtlas);
	Window* window2 = windowManager->GetWindowInstance("Second Window", 640, 480, window, globalFontAtlas);

	if (!OpenGLHelper::InitializeGLEW())
		return -1;

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

		float vertexCoords[]
		{
			//	Vertex Coords	| Vertex Attrib Pointer
			   -115.0f,	-49.0f,	// 0
				115.0f,	-49.0f,	// 1
				115.0f,	49.0f,	// 2
			   -115.0f,	49.0f	// 3
		};

		float vertexCoords2[]
		{
			//	Vertex Coords	| Vertex Attrib Pointer
				-0.5, -0.5,		// 0
				 0.5, -0.5,		// 1
				 0.5,  0.5,		// 2
				-0.5,  0.5		// 3
		};

		float textureCoords[]
		{
			//	Texture Coords	| Vertex Attrib Pointer
				0.0f, 0.0f,		// 0
				1.0f, 0.0f,		// 1
				1.0f, 1.0f,		// 2
				0.0f, 1.0f		// 3
		};

		unsigned int indices[]
		{
			0,1,2,
			2,3,0
		};

		GameVastu* cameraVastu = gameVastuManager->CreateGameVastu();
		Camera* camera = new Camera(window1->GetWindowWidth(), window1->GetWindowHeight());
		cameraVastu->AddComponent(*camera);
		GameVastu* cameraVastu2 = gameVastuManager->CreateGameVastu();
		Camera* camera2 = new Camera(window2->GetWindowWidth(), window2->GetWindowHeight());
		cameraVastu2->AddComponent(*camera2);

		GameVastu* textureVastu = gameVastuManager->CreateGameVastu();
		Renderer* textureRenderer = new Renderer(*vertexCoords, ARRAYSIZE(vertexCoords), *textureCoords, ARRAYSIZE(textureCoords), *indices, ARRAYSIZE(indices), false);
		textureVastu->AddComponent(*textureRenderer);

		GameVastu* colorVastu = gameVastuManager->CreateGameVastu();
		Renderer* simpleRenderer = new Renderer(*vertexCoords2, ARRAYSIZE(vertexCoords2), *indices, ARRAYSIZE(indices), false);
		colorVastu->AddComponent(*simpleRenderer);

		ShaderAsset shaderAsset = shaderManager->CreateShaderAsset("resources/shaders/Basic.shader");
		ShaderAsset shaderAsset2 = shaderManager->CreateShaderAsset("resources/shaders/Basic2.shader");

		Shader shader(shaderManager->LoadShader(shaderAsset));
		Shader shader2(shaderManager->LoadShader(shaderAsset2));

		Texture texture("resources/textures/Opengl.png");
		texture.Bind();
		shader.Bind();
		shader.SetUniform1i("u_Texture", 0);
		shader.UnBind();
		texture.UnBind();

		textureRenderer->SetShader(shader);
		textureRenderer->SetTexture(texture);
		simpleRenderer->SetShader(shader2);

		Transform* textureTransform = textureVastu->m_transform;
		Transform* colorTransform = colorVastu->m_transform;

		window1->SetCamera(*camera);
		window2->SetCamera(*camera2);

		// Register Ordering Matters
		window1->RegisterGameVastu(*colorVastu);
		window1->RegisterGameVastu(*textureVastu);
		window2->RegisterGameVastu(*colorVastu);
		window2->RegisterGameVastu(*textureVastu);

		float time = 0.0f;
		float timeDelta = 0.00694444f;

		float r = 0.0f;
		float increment = 0.05f;

		Action UpdateAction = [&r, &increment, &shader2]()
		{
			shader2.Bind();
			shader2.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			shader2.UnBind();

			if (r > 1)
				increment = -0.05f;
			else if (r < 0)
				increment = 0.05f;

			r += increment;
		};

		bool show_demo_window = false;
		bool show_another_window = true;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		std::function<void(const Window&)> GUIUpdateAction = [&textureTransform, &colorTransform](const Window& window)
		{
			Transform* cameraTransform = window.GetCamera().gameVastu->m_transform;
			ImGui::Begin("Camera");
			ImGui::Text("Tranform: ");
			glm::vec3 campos = cameraTransform->GetPosition();
			float* camposArr = glm::value_ptr(campos);
			if (ImGui::DragFloat3("P", camposArr))
			{
				campos = glm::make_vec3(camposArr);
				cameraTransform->SetPosition(campos);
				std::cout << glm::to_string(campos) << std::endl;
			}
			glm::vec3 camrot = cameraTransform->GetRotation();
			float* camrotArr = glm::value_ptr(camrot);
			if (ImGui::DragFloat3("R", camrotArr, 0.5f, 0.0f, 180.0f))
			{
				camrot = glm::make_vec3(camrotArr);
				cameraTransform->SetRotation(camrot);
				std::cout << glm::to_string(camrot) << std::endl;
			}
			glm::vec3 camscale = cameraTransform->GetScale();
			float* camscaleArr = glm::value_ptr(camscale);
			if (ImGui::DragFloat3("S", camscaleArr))
			{
				camscale = glm::make_vec3(camscaleArr);
				cameraTransform->SetScale(camscale);
				std::cout << glm::to_string(camscale) << std::endl;
			}
			ImGui::End();

			ImGui::Begin("Texture");
			ImGui::Text("Tranform");
			glm::vec3 texpos = textureTransform->GetPosition();
			float* texposArr = glm::value_ptr(texpos);
			if (ImGui::DragFloat3("P", texposArr))
				textureTransform->SetPosition(glm::make_vec3(texposArr));
			glm::vec3 texrot = textureTransform->GetRotation();
			float* texrotArr = glm::value_ptr(texrot);
			if (ImGui::DragFloat3("R", texrotArr, 0.5f, 0.0f, 180.0f))
				textureTransform->SetRotation(glm::make_vec3(texrotArr));
			glm::vec3 texscale = textureTransform->GetScale();
			float* texscaleArr = glm::value_ptr(texscale);
			if (ImGui::DragFloat3("S", texscaleArr))
				textureTransform->SetScale(glm::make_vec3(texscaleArr));
			ImGui::End();

			ImGui::Begin("Color");
			ImGui::Text("Tranform");
			glm::vec3 colpos = colorTransform->GetPosition();
			float* colposArr = glm::value_ptr(colpos);
			if (ImGui::DragFloat3("P", colposArr))
				colorTransform->SetPosition(glm::make_vec3(colposArr));
			glm::vec3 colrot = colorTransform->GetRotation();
			float* colrotArr = glm::value_ptr(colrot);
			if (ImGui::DragFloat3("R", colrotArr, 0.5f, 0.0f, 180.0f))
				colorTransform->SetRotation(glm::make_vec3(colrotArr));
			glm::vec3 colscale = colorTransform->GetScale();
			float* colscaleArr = glm::value_ptr(colscale);
			if (ImGui::DragFloat3("S", colscaleArr))
				colorTransform->SetScale(glm::make_vec3(colscaleArr));
			ImGui::End();
		};

		windowManager->UpdateActions.push_back(UpdateAction);
		windowManager->UpdateActionsRenderer.push_back(GUIUpdateAction);

		windowManager->RenderLoop();
	}

	delete window1;
	delete window2;

	OpenGLHelper::TerminateGLFW();

	return 0;
}