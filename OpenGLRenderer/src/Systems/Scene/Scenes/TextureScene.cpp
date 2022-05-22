#include "TextureScene.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "../../Window/WindowManager.h"

scene::TextureScene::TextureScene() :
	Scene("TextureScene")
{
	vertexCoords = new float[8]
	{
		//	Vertex Coords	| Vertex Attrib Pointer
		   -115.0f, -49.0f,  // 0
			115.0f, -49.0f,	 // 1
			115.0f,  49.0f,	 // 2
		   -115.0f,  49.0f	 // 3
	};

	textureCoords = new float[8]
	{
		//	Texture Coords	| Vertex Attrib Pointer
			0.0f, 0.0f,		// 0
			1.0f, 0.0f,		// 1
			1.0f, 1.0f,		// 2
			0.0f, 1.0f		// 3
	};

	indices = new unsigned int[6]
	{
		0, 1, 2,
		2, 3, 0
	};

	textureVastu = GameVastuManager::getInstance()->CreateGameVastu();
	textureRenderer = new Renderer(*vertexCoords, 8, *textureCoords, 8, *indices, 6, false);
	shader = new Shader(ShaderManager::getInstance()->LoadShader("resources/shaders/Texture.shader"));
	texture = new Texture("resources/textures/Opengl.png");
	textureVastu->AddComponent(*textureRenderer);

	texture->Bind();
	shader->Bind();
	shader->SetUniform1i("u_Texture", 0);
	shader->UnBind();
	texture->UnBind();

	textureRenderer->SetShader(*shader);
	textureRenderer->SetTexture(*texture);
}

scene::TextureScene::~TextureScene()
{
	OnDestroy();
}

void scene::TextureScene::Update()
{
}

void scene::TextureScene::Render(const glm::mat4 vp)
{
}

void scene::TextureScene::OnGUI()
{
	Transform* textureTransform = textureVastu->m_transform;

	ImGui::Begin("Texture");
	ImGui::Text("Tranform");
	glm::vec3 texpos = textureTransform->GetPosition();
	float* texposArr = glm::value_ptr(texpos);
	if (ImGui::DragFloat3("P", texposArr))
		textureTransform->SetPosition(glm::make_vec3(texposArr));
	glm::vec3 texrot = textureTransform->GetRotation();
	float* texrotArr = glm::value_ptr(texrot);
	if (ImGui::DragFloat3("R", texrotArr))
		textureTransform->SetRotation(glm::make_vec3(texrotArr));
	glm::vec3 texscale = textureTransform->GetScale();
	float* texscaleArr = glm::value_ptr(texscale);
	if (ImGui::DragFloat3("S", texscaleArr))
		textureTransform->SetScale(glm::make_vec3(texscaleArr));
	ImGui::End();
}

void scene::TextureScene::OnDestroy()
{
	delete[] vertexCoords;
	delete[] textureCoords;
	delete[] indices;
	delete shader;
	delete texture;
	delete textureRenderer;
	GameVastuManager::getInstance()->DestroyGameVastu(textureVastu);
}
