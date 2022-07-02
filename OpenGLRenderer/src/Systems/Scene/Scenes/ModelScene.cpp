#include "ModelScene.h"
#include <Window/WindowManager.h>
#include <Shader/ShaderManager.h>
#include <GameVastu/GameVastu.h>
#include <Camera/Camera.h>
#include <CameraController/CameraController.h>
#include <Renderer/Renderer.h>

using namespace core;
using namespace core::gl;

namespace scene
{
	ModelScene::ModelScene() :
		Scene("ModelScene")
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
		camera->SetOrtho(false);

		texture1 = new Texture("resources/textures/brickwall.png");

		shader = ShaderManager::getInstance()->LoadShader("resources/shaders/SimpleLit.shader");

		modelVastu1 = CreateGameVastu();
		modelVastu1->m_name = "Spider";
		mesh1 = new Mesh("resources/models/spider.obj");
		mesh1->SetShader(*shader);
		modelVastu1->AddComponent(*mesh1);
		modelVastu1->m_transform->SetScale({ 0.05f, 0.05f, 0.05f });

		modelVastu2 = CreateGameVastu();
		modelVastu2->m_name = "Cube";
		mesh2 = new Mesh("resources/models/box.obj");
		mesh2->SetShader(*shader);
		modelVastu2->AddComponent(*mesh2);
		//modelVastu1->m_transform->SetScale({ 0.05f, 0.05f, 0.05f });

		vertices = new Vertex[24]
		{
			{ {-0.5,-0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 2 }, // 0
			{ {-0.5, 0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 2 }, // 1
			{ { 0.5, 0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 2 }, // 2
			{ { 0.5,-0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 2 }, // 3

			{ { 0.5,-0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 2 }, // 4
			{ { 0.5, 0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 2 }, // 5
			{ { 0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 2 }, // 6
			{ { 0.5,-0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 2 }, // 7

			{ { 0.5,-0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 2 }, // 8
			{ { 0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 2 }, // 9
			{ {-0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 2 }, // 10
			{ {-0.5,-0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 2 }, // 11

			{ {-0.5,-0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 2 }, // 12
			{ {-0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 2 }, // 13
			{ {-0.5, 0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 2 }, // 14
			{ {-0.5,-0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 2 }, // 15

			{ {-0.5, 0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 2 }, // 16
			{ {-0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 2 }, // 17
			{ { 0.5, 0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 2 }, // 18
			{ { 0.5, 0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 2 }, // 19

			{ { 0.5,-0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}, 2 }, // 20
			{ { 0.5,-0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}, 2 }, // 21
			{ {-0.5,-0.5, 0.5 }, { 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}, 2 }, // 22
			{ {-0.5,-0.5,-0.5 }, { 1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}, 2 }  // 23
		};

		indices = new unsigned int[36]
		{
			// Front
			0, 1, 2,
				2, 3, 0,

				// Right
				4, 5, 6,
				6, 7, 4,

				// Back
				8, 9, 10,
				10, 11, 8,

				// Left
				12, 13, 14,
				14, 15, 12,

				// Top
				16, 17, 18,
				18, 19, 16,

				// Bottom
				20, 21, 22,
				22, 23, 20
		};

		//textureVastu1 = CreateGameVastu();
		//textureVastu1->m_name = "Texture Renderer 1";
		//renderer1 = new Renderer(vertices, 24, indices, 36);
		//renderer1->SetShader(*shader);
		//renderer1->AddTexture(samplers[1], *texture1);
		//textureVastu1->AddComponent(*renderer1);
	}

	ModelScene::~ModelScene()
	{
		ShaderManager::getInstance()->UnLoadShader(shader);

		if (texture1) delete texture1;

		delete[] vertices;
		delete[] indices;

		DestroyGameVastu(modelVastu1);
		DestroyGameVastu(modelVastu2);
		DestroyGameVastu(textureVastu1);
		DestroyGameVastu(cameraVastu);
	}
}