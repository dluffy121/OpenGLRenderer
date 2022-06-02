#include "TextureScene.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/ShaderManager.h>
#include <Component/Renderer/Renderer.h>
#include <Component/Camera/Camera.h>
#include <GameVastu/GameVastu.h>
#include "../../../Macros.h"

namespace scene
{
	TextureScene::TextureScene() :
		Scene("TextureScene")
	{
		cameraVastu = CreateGameVastu();
		cameraVastu->m_name = "Camera";
		auto window = WindowManager::getInstance()->GetCurrentWindow();
		camera = new Camera(window->GetWindowWidth(), window->GetWindowHeight());
		cameraVastu->AddComponent(*camera);

		texture1 = new Texture("resources/textures/Opengl.png");
		texture2 = new Texture("resources/textures/test.png");

		shader = ShaderManager::getInstance()->LoadShader("resources/shaders/Texture.shader");

		int samplers[2] = { 0, 1 };
		shader->Bind();
		shader->SetUniform1iv("u_Textures", samplers, 2);
		shader->UnBind();

		int i = 0;

		indices = new unsigned int[6]
		{
			0, 1, 2,
				2, 3, 0
		};

		vertices1 = new Vertex[4]
		{
			{ {   0.0f,   0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, 1 },
			{ {   0.0f,  98.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 1 },
			{ { 230.0f,  98.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 1 },
			{ { 230.0f,   0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, 1 }
		};

		textureVastu1 = CreateGameVastu();
		textureVastu1->m_name = "Texture Renderer 1";
		textureRenderer1 = new Renderer(vertices1, 4, indices, 6);
		textureRenderer1->SetShader(*shader);
		textureRenderer1->AddTexture(samplers[i++], *texture1);
		textureVastu1->AddComponent(*textureRenderer1);

		vertices2 = new Vertex[4]
		{
			{ {   0.0f,   0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, 2 },
			{ {   0.0f,  98.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 2 },
			{ { 500.0f,  98.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 2 },
			{ { 500.0f,   0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, 2 }
		};

		textureVastu2 = CreateGameVastu();
		textureVastu2->m_name = "Texture Renderer 2";
		textureRenderer2 = new Renderer(vertices2, 4, indices, 6);
		textureRenderer2->SetShader(*shader);
		textureRenderer2->AddTexture(samplers[i++], *texture2);
		textureVastu2->AddComponent(*textureRenderer2);
	}

	TextureScene::~TextureScene()
	{
		ShaderManager::getInstance()->UnLoadShader(shader);
		delete[] indices;
		delete[] vertices1;
		delete[] vertices2;

		DestroyGameVastu(textureVastu1);
		DestroyGameVastu(textureVastu2);
		DestroyGameVastu(cameraVastu);
	}
}