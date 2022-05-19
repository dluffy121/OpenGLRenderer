#include "Camera.h"
#include "../../GameVastu/GameVastu.h"

Camera::Camera() :
	m_OrthoMultiplier(1.0f),
	m_NearClipPlane(0.0f),
	m_FarClipPlane(1000.0f),
	m_Left(-1.0f),
	m_Right(1.0f),
	m_Bottom(-1.0f),
	m_Top(1.0f)
{
	m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_FarClipPlane, m_NearClipPlane);	// Providing int to this method does not produce desired output
}

Camera::Camera(float windowWidth, float windowHeight) :
	m_OrthoMultiplier(1.0f),
	m_NearClipPlane(0.0f),
	m_FarClipPlane(1000.0f)
{
	float halfWidth = windowWidth / 2.0f;
	float halfHeight = windowHeight / 2.0f;
	m_Left = -halfWidth;
	m_Right = halfWidth;
	m_Bottom = -halfHeight;
	m_Top = halfHeight;
	m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_FarClipPlane, m_NearClipPlane);	// Providing int to this method does not produce desired output
}

Camera::~Camera()
{}

glm::mat4 Camera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	return gameVastu->m_transform->GetPositionMatrix() * gameVastu->m_transform->GetRotationMatrix();
}
