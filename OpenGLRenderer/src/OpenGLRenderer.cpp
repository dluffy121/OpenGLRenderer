#include "Logger.h"
#include "OpenGLHelper.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Window.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Macros.h"
#include "Renderer.h"
#include <glm/gtx/string_cast.hpp>
#include "GameVastuManager.h"

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

	GLLog(glEnable(GL_BLEND));
	GLLog(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Window* window1 = windowManager->GetWindowInstance("Main Window", 1280, 960, window);
	Window* window2 = windowManager->GetWindowInstance("Second Window", 640, 480, window);

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
				0.0f,	0.0f,	// 0
				230.0f,	0.0f,	// 1
				230.0f,	98.0f,	// 2
				0.0f,	98.0f	// 3
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
		Camera* camera = new Camera();
		cameraVastu->AddComponent(*camera);

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

		window1->SetCamera(*camera);
		window2->SetCamera(*camera);

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


		windowManager->UpdateActions.push_back(UpdateAction);
		//windowManager->UpdateActionsRenderer.push_back(UpdateActionRenderer);

		windowManager->RenderLoop();
	}

	delete window1;
	delete window2;

	OpenGLHelper::TerminateGLFW();
	return 0;
}