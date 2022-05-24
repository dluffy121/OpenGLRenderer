#pragma once

#include "../Scene.h"

class GameVastu;
class Renderer;
class Shader;
class Texture;

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

		void OnGUI() override;
		void OnDestroy() override;
	};
}