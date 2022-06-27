#include "HeirarchyWindow.h"
#include <Window/Window.h>
#include <unordered_map>
#include <Scene/Scene.h>
#include <GameVastu/GameVastu.h>

using namespace scene;

HeirarchyWindow::HeirarchyWindow() :
	GUIWindow("HeirarchyWindow")
{}

HeirarchyWindow::~HeirarchyWindow()
{}

void HeirarchyWindow::Draw(Window* window)
{
	ImGui::Begin("Heirarchy");

	std::unordered_map<std::string, Scene*>& scenes = window->GetAllScenes();

	GameVastu* seletedGameVastu = window->SelectedGameVastu;

	for (auto& scene : scenes)
	{
		if (ImGui::CollapsingHeader(scene.first.c_str()))
		{
			ImGui::Indent();
			for (auto& gameVastu : scene.second->m_GameVastus)
			{
				if (seletedGameVastu && seletedGameVastu->Id == gameVastu->Id)
				{
					ImGui::Selectable(gameVastu->m_name.c_str(), true);
					continue;
				}

				if (ImGui::Selectable(gameVastu->m_name.c_str(), false))
					window->SelectedGameVastu = gameVastu;
			}
			ImGui::Unindent();
		}
	}

	ImGui::End();
}
