#include "CameraController.h"
#include <GameVastu/GameVastu.h>

CameraController::CameraController() :
	m_MoveSpeed(0.1f),
	m_ZoomSpeed(0.1f),
	m_delta(0.1f)
{}

CameraController::~CameraController()
{}

void CameraController::Update()
{
	auto transform = gameVastu->m_transform;
	auto window = WindowManager::getInstance()->GetCurrentWindow();

	auto pos = transform->GetPosition();
	auto rot = transform->GetRotation();
	auto rotMat = gameVastu->m_transform->GetRotationMatrix();
	glm::vec3 right = rotMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 up = rotMat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec3 forward = rotMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

	bool alt = window->GetKeyPressed(GLFW_KEY_LEFT_ALT) || window->GetKeyPressed(GLFW_KEY_RIGHT_ALT);

	if (window->GetKeyPressed(GLFW_KEY_W))
	{
		if (alt)
			transform->Rotate({ m_delta, 0.0f, 0.0f });
		else
			pos += up * m_MoveSpeed;

	}
	else if (window->GetKeyPressed(GLFW_KEY_S))
	{
		if (alt)
			transform->Rotate({ -m_delta, 0.0f, 0.0f });
		else
			pos -= up * m_MoveSpeed;
	}

	if (window->GetKeyPressed(GLFW_KEY_D))
	{
		if (alt)
			transform->Rotate({ 0.0f, m_delta, 0.0f });
		else
			pos += right * m_MoveSpeed;
	}
	else if (window->GetKeyPressed(GLFW_KEY_A))
	{
		if (alt)
			transform->Rotate({ 0.0f, -m_delta, 0.0f });
		else
			pos -= right * m_MoveSpeed;
	}

	if (window->GetKeyPressed(GLFW_KEY_Q))
	{
		if (alt)
			transform->Rotate({ 0.0f, 0.0f, m_delta });
	}
	else if (window->GetKeyPressed(GLFW_KEY_E))
	{
		if (alt)
			transform->Rotate({ 0.0f, 0.0f, -m_delta });
	}

	pos += forward * window->GetScroll().y * m_MoveSpeed;

	transform->SetPosition(pos);
}

void CameraController::OnInspectorGUI()
{
	ImGui::DragFloat("Move Speed: ", &m_MoveSpeed, 0.01f);
	ImGui::DragFloat("Zoom Speed: ", &m_ZoomSpeed, 0.01f);
}