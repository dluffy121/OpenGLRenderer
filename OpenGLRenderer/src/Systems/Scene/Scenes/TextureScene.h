#pragma once

#include "../Scene.h"
#include "../../GameVastu/GameVastuManager.h"
#include "../../Shader/ShaderManager.h"
#include "../../Component/Renderer/Renderer.h"

namespace scene
{
	class TextureScene : public Scene
	{
	private:
		float* vertexCoords;
		float* textureCoords;
		unsigned int* indices;

		GameVastu* textureVastu;
		Renderer* textureRenderer;

		Shader* shader;
		Texture* texture;

	public:
		TextureScene();
		~TextureScene();

		void Update() override;
		void Render(const glm::mat4 vp) override;
		void OnGUI() override;
		void OnDestroy() override;
	};
}