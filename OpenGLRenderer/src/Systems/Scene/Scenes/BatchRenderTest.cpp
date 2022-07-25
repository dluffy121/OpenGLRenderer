#include "BatchRenderTest.h"
#include <Window/WindowManager.h>
#include <Shader/ShaderManager.h>
#include <GameVastu/GameVastu.h>
#include <Camera/Camera.h>
#include <CameraController/CameraController.h>

using namespace core;
using namespace core::gl;

namespace scene
{
	BatchRenderTest::BatchRenderTest() :
		Scene("BatchRenderTest")
	{
		cameraVastu = CreateGameVastu();
		cameraVastu->m_name = "Camera";
		auto window = WindowManager::getInstance()->GetCurrentWindow();
		camera = new Camera(window->GetWindowWidth(), window->GetWindowHeight());
		cameraVastu->AddComponent(*camera);
		CameraController* cameraController = new CameraController();
		cameraVastu->AddComponent(*cameraController);
		cameraVastu->m_transform->SetPosition({ 0.0f, 1.5f, -6.f });
		cameraVastu->m_transform->SetRotation({ 0.0f, 180.0f, 0.f });
		camera->SetOrtho(false);

		light1 = CreateGameVastu();
		light1->m_name = "Directional Light";
		dirLight = new DirectionalLight();
		light1->AddComponent(*dirLight);
		dirLight->m_Color = { 1.0f, 1.0f, 1.0f };
		auto light1Color = dirLight->m_Color;
		light1->m_transform->SetRotation({ 45.0f, 45.0f, 0.f });

		light2 = CreateGameVastu();
		light2->m_name = "Point Light";
		pointLight = new PointLight();
		light2->AddComponent(*pointLight);
		pointLight->m_Color = { 0.0f, 0.0f, 1.0f };
		auto light2Color = pointLight->m_Color;
		light2->m_transform->SetPosition({ 0.0f, 1.0f, 0.0f });

		light3 = CreateGameVastu();
		light3->m_name = "Spot Light";
		spotLight = new SpotLight();
		light3->AddComponent(*spotLight);
		spotLight->m_Color = { 0.0f, 1.0f, 1.0f };
		auto light3Color = spotLight->m_Color;
		light3->m_transform->SetPosition({ -2.0f, 1.0f, -4.0f });

		shader = ShaderManager::getInstance()->LoadShader("resources/shaders/SimpleLit.shader");

		material1 = new Material();
		material1->m_DiffuseTexture = new Texture("resources/textures/hardfur.png");
		material1->m_Shader = shader;

		material2 = new Material();
		material2->m_DiffuseTexture = new Texture("resources/textures/hardfur.png");
		material2->m_Shader = shader;

		//mesh1Vastu = CreateGameVastu();
		//mesh1Vastu->m_name = "Barrel 1";
		//mesh1 = new MeshRenderer("resources/models/Barrel/wine_barrel_01_4k.obj");
		//mesh1->SetShader(*shader);
		//mesh1Vastu->AddComponent(*mesh1);
		//mesh1Vastu->m_transform->SetPosition({ -1.5f, 0.0f, 0.0f });
		//mesh1Vastu->m_transform->SetScale({ 2.0f, 2.0f, 2.0f });

		//mesh2Vastu = CreateGameVastu();
		//mesh2Vastu->m_name = "Barrel 2";
		//mesh2 = new MeshRenderer("resources/models/Barrel/wine_barrel_01_4k.obj");
		//mesh2->SetShader(*shader);
		//mesh2Vastu->AddComponent(*mesh2);
		//mesh2Vastu->m_transform->SetPosition({ 1.5f, 0.0f, 0.0f });
		//mesh2Vastu->m_transform->SetScale({ 2.0f, 2.0f, 2.0f });

		Vertex* vertices = new Vertex[8]
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

		unsigned int* indices = new unsigned int[36]
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

		boxVastu1 = CreateGameVastu();
		boxVastu1->m_name = "Box 1";
		boxRenderer1 = new Renderer(vertices, 8, indices, 36, true);
		boxRenderer1->AddMaterial(material1);
		boxVastu1->AddComponent(*boxRenderer1);
		boxVastu1->m_transform->SetPosition({ -1.0f, 0.0f, 0.0f });

		boxVastu2 = CreateGameVastu();
		boxVastu2->m_name = "Box 2";
		boxRenderer2 = new Renderer(vertices, 8, indices, 36, true);
		boxRenderer2->AddMaterial(material1);
		boxVastu2->AddComponent(*boxRenderer2);
		boxVastu2->m_transform->SetPosition({ 1.0f, 0.0f, 0.0f });
	}

	BatchRenderTest::~BatchRenderTest()
	{
		delete material1;
		delete material2;

		ShaderManager::getInstance()->UnLoadShader(shader);

		DestroyGameVastu(mesh1Vastu);
		DestroyGameVastu(mesh2Vastu);
		DestroyGameVastu(boxVastu1);
		DestroyGameVastu(boxVastu2);
		DestroyGameVastu(light1);
		DestroyGameVastu(light2);
		DestroyGameVastu(light3);
		DestroyGameVastu(cameraVastu);
	}

	void BatchRenderTest::Update()
	{
		//mesh1Vastu->m_transform->RotateAroundYAxis(0.5f);
		//mesh2Vastu->m_transform->RotateAroundYAxis(0.2f);

		//light1->m_transform->RotateAroundYAxis(-0.2f);
		//light2->m_transform->RotateAroundYAxis(0.2f);
		light3->m_transform->RotateAroundYAxis(1.0f);
	}
}