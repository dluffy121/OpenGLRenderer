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
		Vec4 color1{ 0.8f,0.25f,0.1f,1.0f };
		Vec4 color2{ 0.1f,0.25f,0.8f,1.0f };

		vertices1 = new Vertex[4]
		{
			{ {   0.0,	  0.0,  0.0f}, color1, {0.0f, 0.0f}, 0 },
			{ {   0.0,	100.0,  0.0f}, color1, {1.0f, 0.0f}, 0 },
			{ { 100.0,  100.0,  0.0f}, color1, {1.0f, 1.0f}, 0 },
			{ { 100.0,	  0.0,  0.0f}, color1, {0.0f, 1.0f}, 0 },
		};

		vertices2 = new Vertex[4]
		{
			{ { 150.0,	  0.0,  0.0f}, color2, {0.0f, 0.0f}, 0 },
			{ { 150.0,	100.0,  0.0f}, color2, {1.0f, 0.0f}, 0 },
			{ { 250.0,  100.0,  0.0f}, color2, {1.0f, 1.0f}, 0 },
			{ { 250.0,	  0.0,  0.0f}, color2, {0.0f, 1.0f}, 0 }
		};

		indices = new unsigned int[6]
		{
			0, 1, 2,
			2, 3, 0,
		};

		increment = timeDelta = 0.00694444f;

		shader = new Shader("resources/shaders/Texture.shader");

		colorVastu1 = CreateGameVastu();
		colorVastu1->m_name = "Color Renderer 1";
		colorRenderer1 = new Renderer(vertices1, 4, indices, 6);
		colorVastu1->AddComponent(*colorRenderer1);
		colorRenderer1->SetShader(*shader);

		colorVastu2 = CreateGameVastu();
		colorVastu2->m_name = "Color Renderer 2";
		colorRenderer2 = new Renderer(vertices1, 4, indices, 6);
		colorVastu2->AddComponent(*colorRenderer2);
		colorRenderer2->SetShader(*shader);

		colorVastu3 = CreateGameVastu();
		colorVastu3->m_name = "Color Renderer 3";
		colorRenderer3 = new Renderer(vertices1, 4, indices, 6);
		colorVastu3->AddComponent(*colorRenderer3);
		colorRenderer3->SetShader(*shader);

		colorVastu4 = CreateGameVastu();
		colorVastu4->m_name = "Color Renderer 4";
		colorRenderer4 = new Renderer(vertices2, 4, indices, 6);
		colorVastu4->AddComponent(*colorRenderer4);
		colorRenderer4->SetShader(*shader);

		colorVastu5 = CreateGameVastu();
		colorVastu5->m_name = "Color Renderer 5";
		colorRenderer5 = new Renderer(vertices2, 4, indices, 6);
		colorVastu5->AddComponent(*colorRenderer5);
		colorRenderer5->SetShader(*shader);

		cameraVastu = CreateGameVastu();
		cameraVastu->m_name = "Camera";
		auto window = WindowManager::getInstance()->GetCurrentWindow();
		camera = new Camera(window->GetWindowWidth(), window->GetWindowHeight());
		cameraVastu->AddComponent(*camera);

		WindowManager::getInstance()->GetCurrentWindow()->SetCamera(camera);
	}

	ColorScene::~ColorScene()
	{
		delete shader;
		delete[] vertices1;
		delete[] indices;
		delete[] vertices2;

		delete colorRenderer1;
		DestroyGameVastu(colorVastu1);
		delete colorRenderer2;
		DestroyGameVastu(colorVastu2);
		delete colorRenderer3;
		DestroyGameVastu(colorVastu3);
		delete colorRenderer4;
		DestroyGameVastu(colorVastu4);
		delete colorRenderer5;
		DestroyGameVastu(colorVastu5);

		auto windowCamera = &WindowManager::getInstance()->GetCurrentWindow()->GetCamera();
		if (windowCamera && windowCamera->GetId() == camera->GetId())
			WindowManager::getInstance()->GetCurrentWindow()->SetCamera(NULL);
		DestroyGameVastu(cameraVastu);
	}

	void ColorScene::Update()
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (vertices1[i].Color.y > 1)
				increment = -timeDelta;
			else if (vertices1[i].Color.y < 0)
				increment = timeDelta;

			vertices1[i].Color.y += increment;
			vertices2[i].Color.y += increment;
		}
	}
}