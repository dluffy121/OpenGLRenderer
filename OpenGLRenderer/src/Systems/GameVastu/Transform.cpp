#include "Transform.h"
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <corecrt_math.h>
#include <imgui/imgui.h>
#include <glm/gtx/string_cast.hpp>

Transform::Transform() :
	m_Position(glm::vec3(0.0f)),
	m_Rotation(glm::vec3(0.0f)),
	m_Scale(glm::vec3(1.0f)),
	m_PositionMatrix(glm::mat4(1)),
	m_RotationMatrix(glm::mat4(1)),
	m_TransformMatrix(glm::mat4(1))
{}

Transform::~Transform()
{}

void Transform::SetPosition(glm::vec3 position)
{
	m_Position = position;

	m_PositionMatrix = glm::translate(glm::mat4(1), m_Position);

	UpdateModelMatrix();
}

void Transform::SetRotation(glm::vec3 rotation)
{
	if (m_Rotation.x != rotation.x)
	{
		glm::vec3 axis = glm::vec3(0.0f);
		axis.x = 1.0f;
		float angle = fmodf(m_Rotation.x - rotation.x, ANGLE_360);
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(angle), axis);
	}
	if (m_Rotation.y != rotation.y)
	{
		glm::vec3 axis = glm::vec3(0.0f);
		axis.y = 1.0f;
		float angle = fmodf(m_Rotation.y - rotation.y, ANGLE_360);
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(angle), axis);
	}
	if (m_Rotation.z != rotation.z)
	{
		glm::vec3 axis = glm::vec3(0.0f);
		axis.z = 1.0f;
		float angle = fmodf(m_Rotation.z - rotation.z, ANGLE_360);
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(angle), axis);
	}

	m_Rotation = rotation;

	UpdateModelMatrix();
}

void Transform::SetScale(glm::vec3 scale)
{
	m_Scale = scale;

	UpdateModelMatrix();
}

void Transform::UpdateModelMatrix()
{
	m_TransformMatrix = glm::scale(m_PositionMatrix * m_RotationMatrix, m_Scale);
}

void Transform::OnInspectorGUI()
{
	if (ImGui::CollapsingHeader("Position Matrix"))
	{
		ImGui::Text(glm::to_string(m_PositionMatrix[0]).c_str());
		ImGui::Text(glm::to_string(m_PositionMatrix[1]).c_str());
		ImGui::Text(glm::to_string(m_PositionMatrix[2]).c_str());
		ImGui::Text(glm::to_string(m_PositionMatrix[3]).c_str());
	}

	if (ImGui::CollapsingHeader("Rotation Matrix"))
	{
		ImGui::Text(glm::to_string(m_RotationMatrix[0]).c_str());
		ImGui::Text(glm::to_string(m_RotationMatrix[1]).c_str());
		ImGui::Text(glm::to_string(m_RotationMatrix[2]).c_str());
		ImGui::Text(glm::to_string(m_RotationMatrix[3]).c_str());
	}

	if (ImGui::CollapsingHeader("Transform Matrix"))
	{
		ImGui::Text(glm::to_string(m_TransformMatrix[0]).c_str());
		ImGui::Text(glm::to_string(m_TransformMatrix[1]).c_str());
		ImGui::Text(glm::to_string(m_TransformMatrix[2]).c_str());
		ImGui::Text(glm::to_string(m_TransformMatrix[3]).c_str());
	}
}
