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

		shader = ShaderManager::getInstance()->LoadShader("resources/shaders/SimpleLit.shader");

		textureMat1 = new Material();
		textureMat1->m_DiffuseTexture = new Texture("resources/textures/Opengl.png");
		textureMat1->m_Shader = shader;
		textureMat2 = new Material();
		textureMat2->m_DiffuseTexture = new Texture("resources/textures/test.png");
		textureMat2->m_Shader = shader;
		textureMat3 = new Material();
		textureMat3->m_DiffuseTexture = new Texture("resources/textures/hardfur.png");
		textureMat3->m_Shader = shader;

		int i = 0;

		float width = (float)textureMat1->m_DiffuseTexture->GetWidth() / (float)textureMat1->m_DiffuseTexture->GetHeight();
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
			{ { -halfwidth, -halfheight, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, 1, {0,1,0} },
			{ { -halfwidth,  halfheight, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 1, {0,1,0} },
			{ {  halfwidth,  halfheight, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 1, {0,1,0} },
			{ {  halfwidth, -halfheight, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, 1, {0,1,0} }
		};

		textureVastu1 = CreateGameVastu();
		textureVastu1->m_name = "Texture Renderer 1";
		textureRenderer1 = new Renderer(vertices1, 4, indices, 6, true);
		textureRenderer1->AddMaterial(textureMat1);
		textureVastu1->AddComponent(*textureRenderer1);

		width = (float)textureMat2->m_DiffuseTexture->GetWidth() / (float)textureMat2->m_DiffuseTexture->GetHeight();
		height = 1 / width;

		vertices2 = new Vertex[4]
		{
			{ {  0.0f,   0.0f,  0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, 2, {0,1,0} },
			{ {  0.0f, height,  0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 2, {0,1,0} },
			{ { width, height,  0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 2, {0,1,0} },
			{ { width,   0.0f,  0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, 2, {0,1,0} }
		};

		textureVastu2 = CreateGameVastu();
		textureVastu2->m_name = "Texture Renderer 2";
		textureRenderer2 = new Renderer(vertices2, 4, indices, 6, true);
		textureRenderer2->AddMaterial(textureMat2);
		textureVastu2->AddComponent(*textureRenderer2);

		vertices3 = new Vertex[8]
		{
			{ {-0.5,-0.5,-0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 1, {0,1,0} }, // 0
			{ {-0.5, 0.5,-0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 1, {0,1,0} }, // 1
			{ { 0.5, 0.5,-0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 1, {0,1,0} }, // 2
			{ { 0.5,-0.5,-0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 1, {0,1,0} }, // 3

			{ {-0.5,-0.5, 0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 1, {0,1,0} }, // 4
			{ {-0.5, 0.5, 0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 1, {0,1,0} }, // 5
			{ { 0.5, 0.5, 0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 1, {0,1,0} }, // 6
			{ { 0.5,-0.5, 0.5 }, {}, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 1, {0,1,0} }  // 7
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
		textureRenderer3 = new Renderer(vertices3, 8, indices3, 36, true);
		textureRenderer3->AddMaterial(textureMat3);
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

		delete textureMat1;
		delete textureMat2;
		delete textureMat3;

		DestroyGameVastu(textureVastu1);
		DestroyGameVastu(textureVastu2);
		DestroyGameVastu(textureVastu3);
		DestroyGameVastu(cameraVastu);
	}
}