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
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	if (!OpenGLHelper::InitializeGLFW())
		return -1;

	ShaderManager* shaderManager = ShaderManager::getInstance();
	RendererManager* rendererManager = RendererManager::getInstance();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = OpenGLHelper::CreateWindow(640, 480, "", NULL, true);
	OpenGLHelper::UseWindow(window);
	OpenGLHelper::SetSwqpInterval(1);

	GLLog(glEnable(GL_BLEND));
	GLLog(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Renderer* renderer1 = rendererManager->GetRendererInstance("Main Window", 640, 480, window);
	Renderer* renderer2 = rendererManager->GetRendererInstance("Second Window", 640, 480, window);

	if (!OpenGLHelper::InitializeGLEW())
		return -1;

	fprintf(stdout, "Status: Using OpenGL version %s\n", glGetString(GL_VERSION));

	{
		float points[]
		{
			//	Vertex Buffer Layouts			| Vertex Attrib Pointer
			//	Vertex Pos		Tex Coords
				-0.5, -0.5,		0.0f, 0.0f,		// 0
				 0.5, -0.5,		1.0f, 0.0f,		// 1
				 0.5,  0.5,		1.0f, 1.0f,		// 2
				-0.5,  0.5,		0.0f, 1.0f		// 3
		};

		unsigned int indices[]
		{
			0,1,2,
			2,3,0
		};

		VertexArray va1;
		VertexArray va2;

		VertexBuffer vb(points, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
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
		renderer1->AddShader(shader);
		renderer2->AddShader(shader);

		Texture texture("resources/textures/Opengl.png");
		renderer1->AddTexture(texture);
		renderer2->AddTexture(texture);
		renderer1->SetOrthoMultiplier(1.5f);
		texture.Bind();
		shader.Bind();
		shader.SetUniform1i("u_Texture", 0);
		shader.UnBind();
		texture.UnBind();

		vb.UnBind();
		ib.UnBind();

		float time = 0.0f;
		float timeDelta = 0.00694444f;

		float r = 0.0f;
		float increment = 0.05f;

		Action UpdateAction = [&r, &increment, &shader, &texture]()
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

		std::function<void(const Renderer&)> UpdateActionRenderer = [&shader](const Renderer& r)
		{
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", r.m_RenderData.Projection);
			shader.UnBind();
		};

		rendererManager->UpdateActions.push_back(UpdateAction);
		rendererManager->UpdateActionsRenderer.push_back(UpdateActionRenderer);

		rendererManager->RenderLoop();
	}

	delete renderer1;
	delete renderer2;

	OpenGLHelper::TerminateGLFW();
	return 0;
}