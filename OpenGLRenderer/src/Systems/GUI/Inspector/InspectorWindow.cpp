#include "InspectorWindow.h"
#include "../../Window/WindowManager.h"
#include "../Systems/GameVastu/GameVastu.h"
#include "../../GameVastu/Transform.h"
#include <glm/gtc/type_ptr.inl>
#include <string>

InspectorWindow::InspectorWindow() :
	GUIWindow("InspectorWindow")
{}

InspectorWindow::~InspectorWindow()
{}

void InspectorWindow::Draw(Window* window)
{
	ImGui::Begin("Inspector");

	ImGui::Separator();

	BatchRenderer& batchRender = *WindowManager::getInstance()->GetCurrentWindow()->GetBatchRenderer();
	ImGui::Checkbox("Batch Renderer", &batchRender.Enable);
	ImGui::Text("Draw Calls: ");
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Text(std::to_string(batchRender.GetDrawCount()).c_str());
	ImGui::Text("Total Draw Calls: ");
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Text(std::to_string(batchRender.GetTotalDrawCalls()).c_str());

	ImGui::Spacing();
	ImGui::Separator();

	GameVastu* gameVastu = WindowManager::getInstance()->SelectedGameVastu();
	if (!gameVastu)
	{
		ImGui::End();
		return;
	}

	Transform* transform = gameVastu->m_transform;

	ImGui::Text("Id:");
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Text(std::to_string(gameVastu->Id).c_str());

	ImGui::Spacing();
	ImGui::Separator();

	{
		transform->OnInspectorGUI();
	}

	ImGui::Spacing();
	ImGui::Separator();

	{
		for (auto& component : gameVastu->m_Components)
		{
			component.second->_OnInspectorGUI();
			ImGui::Separator();
			ImGui::Separator();
		}
	}

	ImGui::End();
}