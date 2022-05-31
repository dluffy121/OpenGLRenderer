#include "Camera.h"
#include "../../GameVastu/GameVastu.h"

Camera::Camera() :
	m_OrthoMultiplier(1.0f),
	m_NearClipPlane(-500.0f),
	m_FarClipPlane(500.0f),
	m_Left(-1.0f),
	m_Right(1.0f),
	m_Bottom(-1.0f),
	m_Top(1.0f)
{
	UpdateProjectionMatrix();
}

Camera::Camera(float windowWidth, float windowHeight) :
	m_OrthoMultiplier(1.0f),
	m_NearClipPlane(-500.0f),
	m_FarClipPlane(500.0f)
{
	float halfWidth = windowWidth / 2.0f;
	float halfHeight = windowHeight / 2.0f;
	m_Left = -halfWidth;
	m_Right = halfWidth;
	m_Bottom = -halfHeight;
	m_Top = halfHeight;
	UpdateProjectionMatrix();
}

Camera::~Camera()
{}

glm::mat4 Camera::GetProjectionMatrix()
{
	UpdateProjectionMatrix();
	return m_ProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	UpdateViewMatrix();
	return m_ViewnMatrix;
}

glm::mat4 Camera::GetVieProjectionwMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::UpdateProjectionMatrix()
{
	m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearClipPlane, m_FarClipPlane);	// Providing int to this method does not produce desired output
}

void Camera::UpdateViewMatrix()
{
	m_ViewnMatrix = gameVastu->m_transform->GetPositionMatrix() * gameVastu->m_transform->GetRotationMatrix();
}

void Camera::SetOrthoMultiplier(float orthoMultiplier)
{
	m_OrthoMultiplier = orthoMultiplier;
}

void Camera::SetNearClipPlane(float nearClipPlane)
{
	m_NearClipPlane = nearClipPlane;
}

void Camera::SetFarClipPlane(float farClipPlane)
{
	m_FarClipPlane = farClipPlane;
}

void Camera::SetLeft(float left)
{
	m_Left = left;
}

void Camera::SetRight(float right)
{
	m_Right = right;
}
void Camera::SetBottom(float bottom)
{
	m_Bottom = bottom;
}

void Camera::SetTop(float top)
{
	m_Top = top;
}

void Camera::OnInspectorGUI()
{
	ImGui::Text("Clip Planes:");
	float nearClipPlane = m_NearClipPlane;
	if (ImGui::DragFloat("Near", &nearClipPlane))
		m_NearClipPlane = nearClipPlane;
	float farClipPlane = m_FarClipPlane;
	if (ImGui::DragFloat("Far", &farClipPlane))
		m_FarClipPlane = farClipPlane;
}
