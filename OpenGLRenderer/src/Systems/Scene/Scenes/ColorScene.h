#pragma once

#include "../Scene.h"

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