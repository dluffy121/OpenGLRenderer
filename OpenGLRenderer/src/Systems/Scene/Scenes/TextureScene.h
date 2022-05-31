#pragma once

#include "../Scene.h"
#include "../Core/GL/Texture/Texture.h"
#include "../Core/Math.h"

using namespace core;
using namespace core::gl;

class Camera;
class GameVastu;
class Renderer;
class Shader;

namespace scene
{
	class TextureScene : public Scene
	{
	private:
		GameVastu* cameraVastu;
		Camera* camera;

		GameVastu* textureVastu1;
		GameVastu* textureVastu2;
		Renderer* textureRenderer1;
		Renderer* textureRenderer2;

		Shader* shader;
		Texture* texture1;
		Texture* texture2;

		unsigned int* indices;
		Vertex* vertices1;
		Vertex* vertices2;

	public:
		TextureScene();
		~TextureScene();
	};
}