#pragma once

#include <Scene/Scene.h>
#include <GL/Texture/Texture.h>
#include <Math/Math.h>

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
		GameVastu* textureVastu3;
		Renderer* textureRenderer1;
		Renderer* textureRenderer2;
		Renderer* textureRenderer3;

		Shader* shader;
		core::gl::Texture* texture1;
		core::gl::Texture* texture2;
		core::gl::Texture* texture3;

		unsigned int* indices;
		core::Vertex* vertices1;
		core::Vertex* vertices2;
		core::Vertex* vertices3;
		unsigned int* indices3;

	public:
		TextureScene();
		~TextureScene();

	private:
		void Update() override;
	};
}