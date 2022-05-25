#include "InspectorWindow.h"
#include "../../Window/WindowManager.h"
#include "../Systems/GameVastu/GameVastu.h"
#include "../../GameVastu/Transform.h"
#include <glm/gtc/type_ptr.inl>

InspectorWindow::InspectorWindow() :
	GUIWindow("InspectorWindow")
{}

InspectorWindow::~InspectorWindow()
{}

void InspectorWindow::Draw(Window* window)
{
	ImGui::Begin("Inspector");

	GameVastu* gameVastu = WindowManager::getInstance()->SelectedGameVastu();
	if (!gameVastu)
	{
		ImGui::End();
		return;
	}

	Transform* transform = gameVastu->m_transform;

	ImGui::Text("Id:");
	std::string id = std::to_string(gameVastu->GetId());
	ImGui::Text(id.c_str());

	ImGui::Separator();

	{
		ImGui::Text("Tranform:");
		glm::vec3 pos = transform->GetPosition();
		float* posArr = glm::value_ptr(pos);
		if (ImGui::DragFloat3("P", posArr))
			transform->SetPosition(glm::make_vec3(posArr));
		//delete posArr;

		glm::vec3 rot = transform->GetRotation();
		float* rotArr = glm::value_ptr(rot);
		if (ImGui::DragFloat3("R", rotArr))
			transform->SetRotation(glm::make_vec3(rotArr));
		//delete rotArr;

		glm::vec3 scale = transform->GetScale();
		float* scaleArr = glm::value_ptr(scale);
		if (ImGui::DragFloat3("S", scaleArr))
			transform->SetScale(glm::make_vec3(scaleArr));
		//delete scaleArr;
	}

	ImGui::Separator();

	{
		for (auto& component : gameVastu->m_Components)
		{
			component.second->OnInspectorGUI();
			ImGui::Separator();
		}
	}

	ImGui::End();
}