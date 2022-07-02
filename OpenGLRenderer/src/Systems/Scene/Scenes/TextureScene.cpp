#include "TextureScene.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/ShaderManager.h>
#include <Renderer/Renderer.h>
#include <Camera/Camera.h>
#include <CameraController/CameraController.h>
#include <GameVastu/GameVastu.h>
#include "../../../Macros.h"

using namespace core;
using namespace core::gl;

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
		CameraController* cameraController = new CameraController();
		cameraVastu->AddComponent(*cameraController);
		cameraVastu->m_transform->SetPosition({ 0.0f, 0.0f, -6.f });
		cameraVastu->m_transform->SetRotation({ 0.0f, 180.0f, 0.f });

		texture1 = new Texture("resources/textures/Opengl.png");
		texture2 = new Texture("resources/textures/test.png");
		texture3 = new Texture("resources/textures/hardfur.png");

		shader = ShaderManager::getInstance()->LoadShader("resources/shaders/Texture.shader");

		int samplers[2] = { 0, 1 };
		shader->Bind();
		shader->SetUniform1iv("u_Textures", samplers, 2);
		shader->UnBind();

		int i = 0;

		float width = (float)texture1->GetWidth() / (float)texture1->GetHeight();
		float height = 1 / width;
		float halfwidth = width / 2.0f;
		float halfheight = height / 2.0f;

		indices = new unsigned int[6]
		{
			0, 1, 2,
				2, 3, 0
		};

		vertices1 = new Vertex[4]
		{
			{ { -halfwidth, -halfheight, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, 1 },
			{ { -halfwidth,  halfheight, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 1 },
			{ {  halfwidth,  halfheight, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 1 },
			{ {  halfwidth, -halfheight, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, 1 }
		};

		textureVastu1 = CreateGameVastu();
		textureVastu1->m_name = "Texture Renderer 1";
		textureRenderer1 = new Renderer(vertices1, 4, indices, 6);
		textureRenderer1->SetShader(*shader);
		textureRenderer1->AddTexture(samplers[i++], *texture1);
		textureVastu1->AddComponent(*textureRenderer1);

		width = (float)texture2->GetWidth() / (float)texture2->GetHeight();
		height = 1 / width;

		vertices2 = new Vertex[4]
		{
			{ {  0.0f,   0.0f,  0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, 2 },
			{ {  0.0f, height,  0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 2 },
			{ { width, height,  0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 2 },
			{ { width,   0.0f,  0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, 2 }
		};

		textureVastu2 = CreateGameVastu();
		textureVastu2->m_name = "Texture Renderer 2";
		textureRenderer2 = new Renderer(vertices2, 4, indices, 6);
		textureRenderer2->SetShader(*shader);
		textureRenderer2->AddTexture(samplers[i++], *texture2);
		textureVastu2->AddComponent(*textureRenderer2);

		vertices3 = new Vertex[8]
		{
			{ {-0.5,-0.5,-0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 1 }, // 0
			{ {-0.5, 0.5,-0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 1 }, // 1
			{ { 0.5, 0.5,-0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 1 }, // 2
			{ { 0.5,-0.5,-0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 1 }, // 3

			{ {-0.5,-0.5, 0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 1 }, // 4
			{ {-0.5, 0.5, 0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 1 }, // 5
			{ { 0.5, 0.5, 0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 1 }, // 6
			{ { 0.5,-0.5, 0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 1 }  // 7
		};

		indices3 = new unsigned int[36]
		{
			// Front
			0, 1, 2,
				2, 3, 0,

				// Top
				1, 5, 6,
				6, 2, 1,

				// Back
				7, 6, 5,
				5, 4, 7,

				// Bottom
				4, 0, 3,
				3, 7, 4,

				// Left
				4, 5, 1,
				1, 0, 4,

				// Right
				3, 2, 6,
				6, 7, 3
		};

		textureVastu3 = CreateGameVastu();
		textureVastu3->m_name = "Texture Renderer 3";
		textureRenderer3 = new Renderer(vertices3, 8, indices3, 36);
		textureRenderer3->SetShader(*shader);
		textureRenderer3->AddTexture(samplers[0], *texture3);
		textureVastu3->AddComponent(*textureRenderer3);
	}

	void TextureScene::Update()
	{
	}

	TextureScene::~TextureScene()
	{
		ShaderManager::getInstance()->UnLoadShader(shader);
		delete[] indices;
		delete[] vertices1;
		delete[] vertices2;
		delete[] vertices3;
		delete[] indices3;

		delete texture1;
		delete texture2;
		delete texture3;

		DestroyGameVastu(textureVastu1);
		DestroyGameVastu(textureVastu2);
		DestroyGameVastu(textureVastu3);
		DestroyGameVastu(cameraVastu);
	}
}