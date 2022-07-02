#include "LightScene.h"
#include <Window/WindowManager.h>
#include <Shader/ShaderManager.h>
#include <GameVastu/GameVastu.h>
#include <Camera/Camera.h>
#include <CameraController/CameraController.h>

using namespace core::gl;

namespace scene
{
	LightScene::LightScene() :
		Scene("LightScene")
	{
		cameraVastu = CreateGameVastu();
		cameraVastu->m_name = "Camera";
		auto window = WindowManager::getInstance()->GetCurrentWindow();
		camera = new Camera(window->GetWindowWidth(), window->GetWindowHeight());
		cameraVastu->AddComponent(*camera);
		CameraController* cameraController = new CameraController();
		cameraVastu->AddComponent(*cameraController);
		cameraVastu->m_transform->SetPosition({ 0.1f, 3.0f, -6.f });
		cameraVastu->m_transform->SetRotation({ 0.0f, 180.0f, 0.f });
		camera->SetOrtho(false);

		light1 = CreateGameVastu();
		light1->m_name = "DirectionalLight1";
		dirLight1 = new DirectionalLight();
		light1->AddComponent(*dirLight1);
		dirLight1->m_Color = { 1.0f, 1.0f, 1.0f };
		dirLight1->m_Intensity = 0.0f;
		auto light1Color = dirLight1->m_Color;
		light1->m_transform->SetRotation({ 0.0f, 70.0f, 0.f });

		light2 = CreateGameVastu();
		light2->m_name = "DirectionalLight2";
		dirLight2 = new DirectionalLight();
		light2->AddComponent(*dirLight2);
		dirLight2->m_Color = { 0.0f, 0.0f, 1.0f };
		dirLight1->m_Intensity = 0.0f;
		auto light2Color = dirLight2->m_Color;
		light2->m_transform->SetRotation({ 0.0f, -70.0f, 0.f });

		shader = ShaderManager::getInstance()->LoadShader("resources/shaders/SimpleLit.shader");

		mesh1Vastu = CreateGameVastu();
		mesh1Vastu->m_name = "Barrel";
		mesh1 = new Mesh("resources/models/Barrel/wine_barrel_01_4k.obj");
		mesh1->SetShader(*shader);
		mesh1Vastu->AddComponent(*mesh1);
		mesh1Vastu->m_transform->SetPosition({ -2.0f, 0.0f, 0.0f });
		mesh1Vastu->m_transform->SetScale({ 2.0f, 2.0f, 2.0f });

		mesh2Vastu = CreateGameVastu();
		mesh2Vastu->m_name = "Chest";
		mesh2 = new Mesh("resources/models/Chest/treasure_chest_4k.obj");
		mesh2->SetShader(*shader);
		mesh2Vastu->AddComponent(*mesh2);
		mesh2Vastu->m_transform->SetPosition({ 2.0f, 0.0f, 0.0f });
		mesh2Vastu->m_transform->SetScale({ 2.0f, 2.0f, 2.0f });
	}

	LightScene::~LightScene()
	{
		ShaderManager::getInstance()->UnLoadShader(shader);

		DestroyGameVastu(mesh1Vastu);
		DestroyGameVastu(mesh2Vastu);
		DestroyGameVastu(mesh3Vastu);
		DestroyGameVastu(cameraVastu);
	}

	void LightScene::Update()
	{
		mesh1Vastu->m_transform->RotateAroundYAxis(0.5f);
		mesh2Vastu->m_transform->RotateAroundYAxis(0.5f);

		//light1->m_transform->RotateAroundYAxis(-0.2f);
		//light2->m_transform->RotateAroundYAxis(0.2f);
	}
}