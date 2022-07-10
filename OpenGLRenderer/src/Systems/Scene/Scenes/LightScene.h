#pragma once

#pragma once

#include <Scene/Scene.h>
#include <MeshRenderer/MeshRenderer.h>
#include <GL/Texture/Texture.h>
#include <Light/DirectionalLight.h>
#include <Light/PointLight.h>
#include <Light/SpotLight.h>

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
		MeshRenderer* mesh1;

		GameVastu* mesh2Vastu;
		MeshRenderer* mesh2;

		GameVastu* light1;
		DirectionalLight* dirLight;

		GameVastu* light2;
		PointLight* pointLight;

		GameVastu* light3;
		SpotLight* spotLight;

		Shader* shader;
		core::gl::Texture* texture1;

	public:
		LightScene();
		~LightScene();

	private:
		void Update() override;
	};
}