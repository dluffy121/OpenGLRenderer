#include "CameraController.h"
#include <GameVastu/GameVastu.h>

CameraController::CameraController() :
	m_MoveSpeed(0.1f),
	m_RotateSpeed(5.0f),
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
	glm::vec3 right = glm::vec3(rotMat[0]);
	right.y = 0.0f;
	glm::vec3 up = glm::vec3(rotMat[1]);
	up.z = 0;
	glm::vec3 forward = glm::vec3(rotMat[2]);

	bool alt = window->GetKeyPressed(GLFW_KEY_LEFT_ALT) || window->GetKeyPressed(GLFW_KEY_RIGHT_ALT);

	if (window->GetKeyPressed(GLFW_KEY_W))
	{
		if (alt)
			transform->RotateAroundXAxis(m_delta * m_RotateSpeed);
		else
			pos += up * m_MoveSpeed;

	}
	else if (window->GetKeyPressed(GLFW_KEY_S))
	{
		if (alt)
			transform->RotateAroundXAxis(-m_delta * m_RotateSpeed);
		else
			pos -= up * m_MoveSpeed;
	}

	if (window->GetKeyPressed(GLFW_KEY_D))
	{
		if (alt)
			transform->RotateAroundYAxis(m_delta * m_RotateSpeed);
		else
			pos -= right * m_MoveSpeed;
	}
	else if (window->GetKeyPressed(GLFW_KEY_A))
	{
		if (alt)
			transform->RotateAroundYAxis(-m_delta * m_RotateSpeed);
		else
			pos += right * m_MoveSpeed;
	}

	if (window->GetKeyPressed(GLFW_KEY_Q))
	{
		if (alt)
			transform->RotateAroundZAxis(m_delta * m_RotateSpeed);
	}
	else if (window->GetKeyPressed(GLFW_KEY_E))
	{
		if (alt)
			transform->RotateAroundZAxis(-m_delta * m_RotateSpeed);
	}

	if (window->GetKeyPressed(GLFW_KEY_KP_ADD))
	{
		if (alt)
			m_RotateSpeed += 0.01f;
		else
			m_MoveSpeed += 0.01f;
	}
	else if (window->GetKeyPressed(GLFW_KEY_KP_SUBTRACT))
	{
		if (alt)
			m_RotateSpeed -= 0.01f;
		else
			m_MoveSpeed -= 0.01f;
	}

	pos -= forward * window->GetScroll().y * m_MoveSpeed;

	transform->SetPosition(pos);
}

void CameraController::OnInspectorGUI()
{
	ImGui::DragFloat("Move Speed: ", &m_MoveSpeed, 0.01f);
	ImGui::DragFloat("Rotate Speed: ", &m_RotateSpeed, 0.01f);
}