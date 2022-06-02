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

	BatchRenderer& batchRender = *WindowManager::getInstance()->GetCurrentWindow()->GetBatchRenderer();
	ImGui::Text("Draw Calls: ");
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Text(std::to_string(batchRender.GetDrawCount()).c_str());
	ImGui::Text("Total Draw Calls: ");
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Text(std::to_string(batchRender.GetTotalDrawCalls()).c_str());

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

	ImGui::Separator();

	{
		ImGui::Text("Tranform:");
		glm::vec3 pos = transform->GetPosition();
		float* posArr = glm::value_ptr(pos);
		if (ImGui::DragFloat3("P", posArr))
			transform->SetPosition(glm::make_vec3(posArr));

		glm::vec3 rot = transform->GetRotation();
		float* rotArr = glm::value_ptr(rot);
		if (ImGui::DragFloat3("R", rotArr))
			transform->SetRotation(glm::make_vec3(rotArr));

		glm::vec3 scale = transform->GetScale();
		float* scaleArr = glm::value_ptr(scale);
		if (ImGui::DragFloat3("S", scaleArr))
			transform->SetScale(glm::make_vec3(scaleArr));

		transform->OnInspectorGUI();
	}

	ImGui::Separator();

	{
		for (auto& component : gameVastu->m_Components)
		{
			component.second->_OnInspectorGUI();
			ImGui::Separator();
		}
	}

	ImGui::End();
}