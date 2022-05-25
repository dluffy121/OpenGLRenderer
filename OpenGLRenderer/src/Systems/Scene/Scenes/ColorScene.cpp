#include "ColorScene.h"
#include "../Systems/Component/Renderer/Renderer.h"
#include "../../GameVastu/GameVastuManager.h"
#include "../../Component/Camera/Camera.h"
#include "../../Window/WindowManager.h"
#include "../Systems/Shader/ShaderManager.h"
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

	colorVastu = GameVastuManager::getInstance()->CreateGameVastu();
	colorRenderer = new Renderer(*vertexCoords, 8, *indices, 6, false);
	shader = new Shader(ShaderManager::getInstance()->LoadShader("resources/shaders/Color.shader"));

	colorVastu->AddComponent(*colorRenderer);

	colorRenderer->SetShader(*shader);
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
{
	Transform* colorTransform = colorVastu->m_transform;

	ImGui::Begin("Color");
	ImGui::Text("Tranform");
	glm::vec3 colpos = colorTransform->GetPosition();
	float* colposArr = glm::value_ptr(colpos);
	if (ImGui::DragFloat3("P", colposArr))
		colorTransform->SetPosition(glm::make_vec3(colposArr));
	glm::vec3 colrot = colorTransform->GetRotation();
	float* colrotArr = glm::value_ptr(colrot);
	if (ImGui::DragFloat3("R", colrotArr))
		colorTransform->SetRotation(glm::make_vec3(colrotArr));
	glm::vec3 colscale = colorTransform->GetScale();
	float* colscaleArr = glm::value_ptr(colscale);
	if (ImGui::DragFloat3("S", colscaleArr))
		colorTransform->SetScale(glm::make_vec3(colscaleArr));
	ImGui::End();
}

void scene::ColorScene::OnDestroy()
{
	delete[] vertexCoords;
	delete[] indices;
	delete shader;
	delete colorRenderer;
	GameVastuManager::getInstance()->DestroyGameVastu(colorVastu);
}