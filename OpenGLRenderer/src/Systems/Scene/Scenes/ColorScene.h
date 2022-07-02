#pragma once

#include <Scene/Scene.h>
#include <Math/Math.h>

class Camera;
class GameVastu;
class Renderer;
class Shader;

namespace scene
{
	class ColorScene : public Scene
	{
	private:
		GameVastu* cameraVastu;
		Camera* camera;

		GameVastu* colorVastu1;
		Renderer* colorRenderer1;
		GameVastu* colorVastu2;
		Renderer* colorRenderer2;
		GameVastu* colorVastu3;
		Renderer* colorRenderer3;
		GameVastu* colorVastu4;
		Renderer* colorRenderer4;
		GameVastu* colorVastu5;
		Renderer* colorRenderer5;

		Shader* shader;

		unsigned int* indices1;
		unsigned int* indices2;
		core::Vertex* vertices1;
		core::Vertex* vertices2;

		float timeDelta;
		float increment;

	public:
		ColorScene();
		~ColorScene();

		void Update() override;
	};
}