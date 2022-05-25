#include "TextureScene.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/ShaderManager.h>
#include <Component/Renderer/Renderer.h>
#include <Component/Camera/Camera.h>
#include <GameVastu/GameVastu.h>

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

	textureVastu = CreateGameVastu();
	textureVastu->m_name = "Texture Renderer";
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

	cameraVastu = CreateGameVastu();
	cameraVastu->m_name = "Camera";
	auto window = WindowManager::getInstance()->GetCurrentWindow();
	camera = new Camera(window->GetWindowWidth(), window->GetWindowHeight());
	cameraVastu->AddComponent(*camera);

	WindowManager::getInstance()->GetCurrentWindow()->SetCamera(*camera);
}

scene::TextureScene::~TextureScene()
{
	OnDestroy();
}

void scene::TextureScene::OnGUI()
{}

void scene::TextureScene::OnDestroy()
{
	delete[] vertexCoords;
	delete[] textureCoords;
	delete[] indices;
	delete shader;
	delete texture;
	delete textureRenderer;
	DestroyGameVastu(textureVastu);
}
