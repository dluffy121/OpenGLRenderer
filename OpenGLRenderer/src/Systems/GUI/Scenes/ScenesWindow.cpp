#include "ScenesWindow.h"
#include "../../Window/Window.h"

ScenesWindow::ScenesWindow() :
	GUIWindow("ScenesWindow")
{}

ScenesWindow::~ScenesWindow()
{
	delete loadString;
	delete unloadString;
}

void ScenesWindow::Draw(Window* window)
{
	ImGui::Begin("Scenes");

	for (auto& scene : m_SceneToggles)
	{
		ImGui::Text(scene.first.c_str());
		ImGui::SameLine();
		ImGui::Indent();
		ImGui::SameLine();
		ImGui::PushID(scene.first.c_str());
		if (!window->IsSceneActive(scene.first))
		{
			if (ImGui::Button(loadString))
				window->AddScene(scene.second());
		}
		else
		{
			if (ImGui::Button(unloadString))
				window->RemoveScene(scene.first);
		}
		ImGui::PopID();
		ImGui::Unindent();
	}

	ImGui::End();
}