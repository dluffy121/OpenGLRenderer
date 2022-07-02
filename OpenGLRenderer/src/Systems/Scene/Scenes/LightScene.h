#pragma once

#pragma once

#include <Scene/Scene.h>
#include <Mesh/Mesh.h>
#include <GL/Texture/Texture.h>
#include <Light/DirectionalLight.h>

class Camera;
class GameVastu;
class Renderer;
class Shader;

namespace scene
{
	class LightScene : public Scene
	{
	private:
		GameVastu* cameraVastu;
		Camera* camera;

		GameVastu* mesh1Vastu;
		Mesh* mesh1;

		GameVastu* mesh2Vastu;
		Mesh* mesh2;

		GameVastu* mesh3Vastu;
		Mesh* mesh3;

		GameVastu* light1;
		DirectionalLight* dirLight1;

		GameVastu* light2;
		DirectionalLight* dirLight2;

		Shader* shader;
		core::gl::Texture* texture1;

	public:
		LightScene();
		~LightScene();

	private:
		void Update() override;
	};
}