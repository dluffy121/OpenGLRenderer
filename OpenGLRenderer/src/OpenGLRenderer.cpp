#include "OpenGLLogger.h"
#include "OpenGLHelper.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include <iostream>
#include "ShaderManager.h"
#include "RendererManager.h"

int main(void)
{
	if (!OpenGLHelper::InitializeGLFW())
		return -1;

	ShaderManager* shaderManager = ShaderManager::getInstance();
	RendererManager* rendererManager = RendererManager::getInstance();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = OpenGLHelper::CreateWindow(640, 480, "", NULL, true);
	OpenGLHelper::UseWindow(window);
	OpenGLHelper::SetSwqpInterval(1);

	Renderer* renderer1 = rendererManager->GetRendererInstance("Main Window", 720, 1280, window);
	Renderer* renderer2 = rendererManager->GetRendererInstance("Second Window", 720, 1280, window);

	if (!OpenGLHelper::InitializeGLEW())
		return -1;

	{
		float points[]
		{
			-0.5, -0.5, //0
			 0.5, -0.5, //1
			 0.5,  0.5, //2
			-0.5,  0.5  //3
		};

		unsigned int indices[]
		{
			0,1,2,
			2,3,0
		};

		VertexArray va1;
		VertexArray va2;

		VertexBuffer vb(points, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);

		va1.AddBuffer(vb, layout);
		va2.AddBuffer(vb, layout);

		renderer1->SetVertexArray(va1);
		renderer2->SetVertexArray(va2);

		IndexBuffer ib(indices, GL_UNSIGNED_INT, 6);
		renderer1->SetIndexBuffer(ib);
		renderer2->SetIndexBuffer(ib);

		ShaderAsset shaderAsset = shaderManager->CreateShaderAsset("resources/shaders/Basic.shader");
		Shader shader(shaderManager->LoadShader(shaderAsset));
		renderer1->SetShader(shader);
		renderer2->SetShader(shader);

		vb.UnBind();
		ib.UnBind();

		float time = 0.0f;
		float timeDelta = 0.00694444f;

		float r = 0.0f;
		float increment = 0.05f;

		Action UpdateAction = [&r, &increment, &shader]()
		{
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			shader.UnBind();

			if (r > 1)
				increment = -0.05f;
			else if (r < 0)
				increment = 0.05f;

			r += increment;
		};

		rendererManager->UpdateActions.push_back(UpdateAction);

		rendererManager->RenderLoop();
	}

	delete renderer1;
	delete renderer2;

	OpenGLHelper::TerminateGLFW();
	return 0;
}