#include "Camera.h"
#include "../../GameVastu/GameVastu.h"
#include <glm/gtx/string_cast.hpp>

Camera::Camera() :
	m_ortho(true),
	m_OrthoMultiplier(1.0f),
	m_NearClipPlane(-500.0f),
	m_FarClipPlane(500.0f),
	m_Left(-1.0f),
	m_Right(1.0f),
	m_Bottom(-1.0f),
	m_Top(1.0f),
	m_frustrumWidth(1.0f),
	m_frustrumHeight(1.0f),
	m_fov(45.0f),
	m_ProjectionMatrix(glm::mat4(1)),
	m_ViewnMatrix(glm::mat4(1))
{}

Camera::Camera(float windowWidth, float windowHeight) :
	m_ortho(true),
	m_NearClipPlane(-500.0f),
	m_FarClipPlane(500.0f),
	m_OrthoMultiplier(100.0f),
	m_Left(-windowWidth / 2.0f),
	m_Right(windowWidth / 2.0f),
	m_Bottom(-windowHeight / 2.0f),
	m_Top(windowHeight / 2.0f),
	m_frustrumWidth(windowWidth),
	m_frustrumHeight(windowHeight),
	m_fov(45.0f),
	m_ProjectionMatrix(glm::mat4(1)),
	m_ViewnMatrix(glm::mat4(1))
{}

Camera::~Camera()
{}

void Camera::Awake()
{
	SetOrtho(m_ortho);
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

void Camera::UpdateProjectionMatrix()
{
	m_ProjectionMatrix = m_ortho ?
		glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearClipPlane, m_FarClipPlane)
		:
		m_ProjectionMatrix = glm::perspective(glm::radians(m_fov), m_frustrumWidth / m_frustrumHeight, m_NearClipPlane, m_FarClipPlane);
}

void Camera::UpdateViewMatrix()
{
	m_ViewnMatrix = gameVastu->m_transform->GetPositionMatrix() * gameVastu->m_transform->GetRotationMatrix();
	m_ViewnMatrix = glm::inverse(m_ViewnMatrix);
}

void Camera::SetOrtho(bool value)
{
	m_ortho = value;
	UpdateProjectionMatrix();
}

void Camera::SetOrthoMultiplier(float orthoMultiplier)
{
	m_OrthoMultiplier = orthoMultiplier;
	UpdateProjectionMatrix();
}

void Camera::SetNearClipPlane(float nearClipPlane)
{
	m_NearClipPlane = nearClipPlane;
	UpdateProjectionMatrix();
}

void Camera::SetFarClipPlane(float farClipPlane)
{
	m_FarClipPlane = farClipPlane;
	UpdateProjectionMatrix();
}

void Camera::SetLeft(float left)
{
	m_Left = left;
	UpdateProjectionMatrix();
}

void Camera::SetRight(float right)
{
	m_Right = right;
	UpdateProjectionMatrix();
}

void Camera::SetBottom(float bottom)
{
	m_Bottom = bottom;
	UpdateProjectionMatrix();
}

void Camera::SetTop(float top)
{
	m_Top = top;
	UpdateProjectionMatrix();
}

void Camera::SetFrustrumWidth(float width)
{
	m_frustrumWidth = width;
	UpdateProjectionMatrix();
}

void Camera::SetFrustrumHeight(float height)
{
	m_frustrumHeight = height;
	UpdateProjectionMatrix();
}

void Camera::SetFOV(float fov)
{
	m_fov = fov;
	UpdateProjectionMatrix();
}

void Camera::OnInspectorGUI()
{
	bool dirty = false;

	dirty |= ImGui::Checkbox("Orthographic: ", &m_ortho);
	if (dirty)
	{
		SetOrtho(m_ortho);
		dirty = false;
	}

	ImGui::Text("Clip Planes:");
	dirty |= ImGui::DragFloat("Near", &m_NearClipPlane);
	dirty |= ImGui::DragFloat("Far", &m_FarClipPlane);

	if (m_ortho)
	{
		ImGui::Text("Orthographic Parameters:");
		dirty |= ImGui::DragFloat("Multiplier", &m_OrthoMultiplier);
		dirty |= ImGui::DragFloat("Left", &m_Left);
		dirty |= ImGui::DragFloat("Right", &m_Right);
		dirty |= ImGui::DragFloat("Bottom", &m_Bottom);
		dirty |= ImGui::DragFloat("Top", &m_Top);
	}
	else
	{
		ImGui::Text("Perspective Parameters:");
		dirty |= ImGui::DragFloat("Width", &m_frustrumWidth);
		dirty |= ImGui::DragFloat("Height", &m_frustrumHeight);
		dirty |= ImGui::DragFloat("FOV", &m_fov, 0.1f, 10.0f, 180.0f);
	}

	if (dirty)
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}

	if (ImGui::CollapsingHeader("Projection Matrix"))
	{
		ImGui::Text(glm::to_string(m_ProjectionMatrix[0]).c_str());
		ImGui::Text(glm::to_string(m_ProjectionMatrix[1]).c_str());
		ImGui::Text(glm::to_string(m_ProjectionMatrix[2]).c_str());
		ImGui::Text(glm::to_string(m_ProjectionMatrix[3]).c_str());
	}

	if (ImGui::CollapsingHeader("View Matrix"))
	{
		ImGui::Text(glm::to_string(m_ViewnMatrix[0]).c_str());
		ImGui::Text(glm::to_string(m_ViewnMatrix[1]).c_str());
		ImGui::Text(glm::to_string(m_ViewnMatrix[2]).c_str());
		ImGui::Text(glm::to_string(m_ViewnMatrix[3]).c_str());
	}
}
