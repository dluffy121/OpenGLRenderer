#pragma once

#include "../Scene.h"
#include "../../GameVastu/GameVastuManager.h"
#include "../../Shader/ShaderManager.h"
#include "../../Component/Renderer/Renderer.h"

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
		void Render(const glm::mat4 vp) override;
		void OnGUI() override;
		void OnDestroy() override;
	};
}