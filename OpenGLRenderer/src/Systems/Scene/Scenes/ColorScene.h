#pragma once

#include "../Scene.h"

class Camera;
class GameVastu;
class Renderer;
class Shader;

namespace scene
{
	class ColorScene : public Scene
	{
	private:
		float* vertexCoords;
		unsigned int* indices;

		GameVastu* cameraVastu;
		Camera* camera;

		GameVastu* colorVastu;
		Renderer* colorRenderer;

		Shader* shader;

		float time ;
		float timeDelta;

		float r;
		float increment;

	public:
		ColorScene();
		~ColorScene();

		void Update() override;
		void OnGUI() override;
		void OnDestroy() override;
	};
}