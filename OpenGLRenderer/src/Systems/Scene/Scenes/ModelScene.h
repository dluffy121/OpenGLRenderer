#pragma once

#include <Scene/Scene.h>
#include <MeshRenderer/MeshRenderer.h>
#include <GL/Texture/Texture.h>

class Camera;
class GameVastu;
class Renderer;
class Shader;

namespace scene
{
	class ModelScene : public Scene
	{
	private:
		GameVastu* cameraVastu;
		Camera* camera;

		GameVastu* modelVastu1;
		MeshRenderer* mesh1;

		GameVastu* modelVastu2;
		MeshRenderer* mesh2;

		GameVastu* textureVastu1;
		Renderer* renderer1;
		core::Vertex* vertices;
		unsigned int* indices;

		Shader* shader;
		core::gl::Texture* texture1;

	public:
		ModelScene();
		~ModelScene();
	};
}