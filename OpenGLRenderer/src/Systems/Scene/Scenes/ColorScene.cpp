#include "ColorScene.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/ShaderManager.h>
#include <Component/Renderer/Renderer.h>
#include <Component/Camera/Camera.h>
#include <GameVastu/GameVastu.h>

scene::ColorScene::ColorScene() :
	Scene("ColorScene")
{
	vertexCoords = new float[8]
	{
		//	Vertex Coords	| Vertex Attrib Pointer
		-0.5, -0.5,		// 0
			0.5, -0.5,		// 1
			0.5, 0.5,		// 2
			-0.5, 0.5		// 3
	};

	indices = new unsigned int[6]
	{
		0, 1, 2,
			2, 3, 0
	};

	time = 0.0f;
	timeDelta = 0.00694444f;

	r = 0.0f;
	increment = 0.05f;

	colorVastu = CreateGameVastu();
	colorVastu->m_name = "Color Renderer";
	colorRenderer = new Renderer(*vertexCoords, 8, *indices, 6, false);
	shader = new Shader(ShaderManager::getInstance()->LoadShader("resources/shaders/Color.shader"));

	colorVastu->AddComponent(*colorRenderer);

	colorRenderer->SetShader(*shader);

	cameraVastu = CreateGameVastu();
	cameraVastu->m_name = "Camera";
	auto window = WindowManager::getInstance()->GetCurrentWindow();
	camera = new Camera(window->GetWindowWidth(), window->GetWindowHeight());
	cameraVastu->AddComponent(*camera);

	WindowManager::getInstance()->GetCurrentWindow()->SetCamera(*camera);
}

scene::ColorScene::~ColorScene()
{
	OnDestroy();
}

void scene::ColorScene::Update()
{
	if (r > 1)
		increment = -0.05f;
	else if (r < 0)
		increment = 0.05f;

	r += increment;

	shader->Bind();
	shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
	shader->UnBind();
}

void scene::ColorScene::OnGUI()
{}

void scene::ColorScene::OnDestroy()
{
	delete[] vertexCoords;
	delete[] indices;
	delete shader;
	delete colorRenderer;
	DestroyGameVastu(colorVastu);
}