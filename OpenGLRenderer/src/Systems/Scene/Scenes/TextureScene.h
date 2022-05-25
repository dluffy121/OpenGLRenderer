#pragma once

#include "../Scene.h"
#include "../Core/GL/Texture/Texture.h"

class Camera;
class GameVastu;
class Renderer;
class Shader;

using namespace core::gl;

namespace scene
{
	class TextureScene : public Scene
	{
	private:
		float* vertexCoords;
		float* textureCoords;
		unsigned int* indices;

		GameVastu* cameraVastu;
		Camera* camera;

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