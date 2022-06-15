#include "Camera.h"
#include "../../GameVastu/GameVastu.h"
#include <glm/gtx/string_cast.hpp>

using namespace core;

Camera::Camera() :
	m_ortho(true),
	m_OrthoMultiplier(1.0f),
	m_NearClipPlane(1.0f),
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
	m_NearClipPlane(1.0f),
	m_FarClipPlane(500.0f),
	m_OrthoMultiplier(1.0f),
	m_Left(-windowWidth / windowHeight),
	m_Right(windowWidth / windowHeight),
	m_Bottom(-1.0f),
	m_Top(1.0f),
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
	std::function<void(Vec3)> s = [this](Vec3 x) { this->UpdateViewMatrix(); };
	gameVastu->m_transform->OnPositionUpdateCallbacks.push_back(s);
	gameVastu->m_transform->OnRotationUpdateCallbacks.push_back(s);

	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

void Camera::UpdateProjectionMatrix()
{
	if (m_ortho)
	{
		float scale_x = 1.f / (m_Right - m_Left);
		float scale_y = 1.f / (m_Top - m_Bottom);
		float scale_z = -1.f / (m_NearClipPlane - m_FarClipPlane);
		float mid_x = -(m_Right + m_Left) / (m_Right - m_Left);
		float mid_y = -(m_Top + m_Bottom) / (m_Top - m_Bottom);
		float mid_z = -(m_FarClipPlane + m_NearClipPlane) / (m_FarClipPlane - m_NearClipPlane);

		m_ProjectionMatrix = glm::mat4 {
			scale_x,	0.f,		0.f,		0.f,
			0.f,		scale_y,	0.f,		0.f,
			0.f,		0.f,		scale_z,	0.f,
			mid_x,		mid_y,		mid_z,		1.f };
	}
	else
	{
		float halfFOV = m_fov / 2.f;
		float tanHalfFOV = glm::tan(glm::radians(halfFOV));

		float ar = m_frustrumWidth / m_frustrumHeight;
		float x = 1.f / (tanHalfFOV * ar);
		float y = 1.f / tanHalfFOV;

		float a = (m_FarClipPlane + m_NearClipPlane) / (m_NearClipPlane - m_FarClipPlane);
		float b = (2.f * m_FarClipPlane * m_NearClipPlane) / (m_FarClipPlane - m_NearClipPlane);

		m_ProjectionMatrix = glm::mat4 {
			x,		0.f,	0.f,	0.f,
			0.f,	y,		0.f,	0.f,
			0.f,	0.f,	a,		1.f,
			0.f,	0.f,	b,		0.f };
	}
}

void Camera::UpdateViewMatrix()
{
	auto pos = gameVastu->m_transform->GetPosition();
	auto eye = glm::vec3(pos.x, pos.y, pos.z);
	auto rot = gameVastu->m_transform->GetRotationMatrix();

	glm::vec3 N = glm::normalize(glm::vec3(rot[2]));
	glm::vec3 U = glm::normalize(glm::cross(N, glm::vec3(rot[1])));
	glm::vec3 V = glm::cross(U, N);

	m_ViewnMatrix[0][0] = U.x;
	m_ViewnMatrix[1][0] = U.y;
	m_ViewnMatrix[2][0] = U.z;
	m_ViewnMatrix[3][0] = -glm::dot(U, eye);

	m_ViewnMatrix[0][1] = V.x;
	m_ViewnMatrix[1][1] = V.y;
	m_ViewnMatrix[2][1] = V.z;
	m_ViewnMatrix[3][1] = -glm::dot(V, eye);	
	
	m_ViewnMatrix[0][2] = -N.x;
	m_ViewnMatrix[1][2] = -N.y;
	m_ViewnMatrix[2][2] = -N.z;
	m_ViewnMatrix[3][2] = glm::dot(N, eye);
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
	dirty |= ImGui::DragFloat("Near", &m_NearClipPlane, 0.01f);
	dirty |= ImGui::DragFloat("Far", &m_FarClipPlane, 0.01f);

	if (m_ortho)
	{
		ImGui::Text("Orthographic Parameters:");
		dirty |= ImGui::DragFloat("Multiplier", &m_OrthoMultiplier, 0.01f);
		dirty |= ImGui::DragFloat("Left", &m_Left, 0.01f);
		dirty |= ImGui::DragFloat("Right", &m_Right, 0.01f);
		dirty |= ImGui::DragFloat("Bottom", &m_Bottom, 0.01f);
		dirty |= ImGui::DragFloat("Top", &m_Top, 0.01f);
	}
	else
	{
		ImGui::Text("Perspective Parameters:");
		dirty |= ImGui::DragFloat("Width", &m_frustrumWidth, 0.01f);
		dirty |= ImGui::DragFloat("Height", &m_frustrumHeight, 0.01f);
		dirty |= ImGui::DragFloat("FOV", &m_fov, 0.01f, 10.0f, 180.0f);
	}

	if (dirty)
		UpdateProjectionMatrix();

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
