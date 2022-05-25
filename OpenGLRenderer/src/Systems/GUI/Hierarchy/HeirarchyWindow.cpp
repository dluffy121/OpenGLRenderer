#include "HeirarchyWindow.h"
#include <Window/Window.h>
#include <unordered_map>
#include <Scene/Scene.h>
#include <GameVastu/GameVastu.h>

HeirarchyWindow::HeirarchyWindow() :
	GUIWindow("HeirarchyWindow")
{}

HeirarchyWindow::~HeirarchyWindow()
{}

void HeirarchyWindow::Draw(Window* window)
{
	ImGui::Begin("Heirarchy");

	std::unordered_map<std::string, scene::Scene*>& scenes = window->GetAllScenes();

	for (auto& scene: scenes)
	{
		ImGui::BeginListBox(scene.first.c_str());

		for (auto& gameVastu : scene.second->m_GameVastus)
		{
			if (window->SelectedGameVastu && window->SelectedGameVastu->GetId() == gameVastu->GetId())
			{
				ImGui::Selectable(gameVastu->m_name.c_str(), true);
				continue;
			}

			if (ImGui::Selectable(gameVastu->m_name.c_str(), false))
				window->SelectedGameVastu = gameVastu;
		}

		ImGui::EndListBox();
	}

	ImGui::End();
}
