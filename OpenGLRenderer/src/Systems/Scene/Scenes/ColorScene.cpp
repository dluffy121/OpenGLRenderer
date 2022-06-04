#include "ColorScene.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/ShaderManager.h>
#include <Component/Renderer/Renderer.h>
#include <Component/Camera/Camera.h>
#include <GameVastu/GameVastu.h>

namespace scene
{
	ColorScene::ColorScene() :
		Scene("ColorScene")
	{
		Vec4 color1{ 0.8f, 0.25f, 0.1f, 1.0f };
		Vec4 color2{ 0.1f, 0.25f, 0.8f, 1.0f };

		vertices1 = new Vertex[8]
		{
			{ { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.5, 1.0}, {0.0, 0.0}, 0 }, // 0
			{ { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0, 1.0}, {1.0, 0.0}, 0 }, // 1
			{ { 1.0, 1.0, 0.0 }, { 0.0, 0.5, 0.5, 1.0}, {1.0, 1.0}, 0 }, // 2
			{ { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0, 1.0}, {0.0, 1.0}, 0 }, // 3

			{ { 0.0, 0.0, 1.0 }, { 0.5, 0.5, 0.0, 1.0}, {0.0, 0.0}, 0 }, // 4
			{ { 0.0, 1.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0}, {1.0, 0.0}, 0 }, // 5
			{ { 1.0, 1.0, 1.0 }, { 0.5, 0.0, 0.0, 1.0}, {1.0, 1.0}, 0 }, // 6
			{ { 1.0, 0.0, 1.0 }, { 1.0, 1.0, 0.0, 1.0}, {0.0, 1.0}, 0 }  // 7
		};

		indices1 = new unsigned int[36]
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

		vertices2 = new Vertex[4]
		{
			{ {   0.0,	  0.0,  0.0f}, color2, {0.0f, 0.0f}, 0 },
			{ {   0.0,	100.0,  0.0f}, color2, {1.0f, 0.0f}, 0 },
			{ { 100.0,  100.0,  0.0f}, color2, {1.0f, 1.0f}, 0 },
			{ { 100.0,	  0.0,  0.0f}, color2, {0.0f, 1.0f}, 0 }
		};

		indices2 = new unsigned int[6]
		{
			0, 1, 2,
				2, 3, 0,
		};

		increment = timeDelta = 0.00694444f;

		shader = ShaderManager::getInstance()->LoadShader("resources/shaders/Texture.shader");

		colorVastu1 = CreateGameVastu();
		colorVastu1->m_name = "Color Renderer 1";
		colorRenderer1 = new Renderer(vertices1, 8, indices1, 36);
		colorVastu1->AddComponent(*colorRenderer1);
		colorRenderer1->SetShader(*shader);
		colorVastu1->m_transform->SetPosition({ -2.0f, 0.0f, 0.f });

		colorVastu2 = CreateGameVastu();
		colorVastu2->m_name = "Color Renderer 2";
		colorRenderer2 = new Renderer(vertices1, 8, indices1, 36);
		colorVastu2->AddComponent(*colorRenderer2);
		colorRenderer2->SetShader(*shader);
		colorVastu2->m_transform->SetPosition({ 1.0f, 0.0f, 0.f });

		cameraVastu = CreateGameVastu();
		cameraVastu->m_name = "Camera";
		auto window = WindowManager::getInstance()->GetCurrentWindow();
		camera = new Camera(window->GetWindowWidth(), window->GetWindowHeight());
		cameraVastu->AddComponent(*camera);
		cameraVastu->m_transform->SetPosition({ 0.0f, 0.0f, -10.f });
	}

	ColorScene::~ColorScene()
	{
		ShaderManager::getInstance()->UnLoadShader(shader);
		delete[] indices1;
		delete[] indices2;
		delete[] vertices1;
		delete[] vertices2;

		DestroyGameVastu(colorVastu1);
		DestroyGameVastu(colorVastu2);
		DestroyGameVastu(colorVastu3);
		DestroyGameVastu(colorVastu4);
		DestroyGameVastu(colorVastu5);
		DestroyGameVastu(cameraVastu);
	}

	void ColorScene::Update()
	{
		//for (size_t i = 0; i < 4; i++)
		//{
		//	if (vertices1[i].Color.y > 1)
		//		increment = -timeDelta;
		//	else if (vertices1[i].Color.y < 0)
		//		increment = timeDelta;

		//	vertices1[i].Color.y += increment;
		//	vertices2[i].Color.y += increment;
		//}
	}
}