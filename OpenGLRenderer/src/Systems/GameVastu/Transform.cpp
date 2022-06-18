#include "Transform.h"
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <corecrt_math.h>
#include <imgui/imgui.h>
#include <glm/gtx/string_cast.hpp>
#include <Math/Math.h>

using namespace core;

Transform::Transform() :
	m_Position(Vec3(0.0f)),
	m_Rotation(Vec3(0.0f)),
	m_Scale(Vec3(1.0f)),
	m_PositionMatrix(glm::mat4(1)),
	m_RotationMatrix(glm::mat4(1)),
	m_TransformMatrix(glm::mat4(1))
{}

Transform::~Transform()
{}

void Transform::SetPosition(Vec3 position)
{
	Vec3 delta = m_Position - position;
	m_Position = position;

	m_PositionMatrix = glm::translate(glm::mat4(1), glm::vec3(m_Position.x, m_Position.y, m_Position.z));

	UpdateModelMatrix();

	OnPositionUpdate(delta);
}

void Transform::SetRotation(Vec3 rotation)
{
	Vec3 delta = m_Rotation - rotation;
	m_Rotation = rotation;
	m_Rotation.x = fmodf(m_Rotation.x, ANGLE_360);
	m_Rotation.y = fmodf(m_Rotation.y, ANGLE_360);
	m_Rotation.z = fmodf(m_Rotation.z, ANGLE_360);

	m_RotationMatrix = glm::rotate(glm::mat4(1), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	UpdateModelMatrix();

	OnRotationUpdate(delta);
}

void Transform::Rotate(float value, Vec3 axis)
{
	Rotate(value, glm::vec3(axis.x, axis.y, axis.z));
}

void Transform::Rotate(float value, glm::vec3 axis)
{
	axis = glm::normalize(axis);
	m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(value), axis);
	Vec3 rotation
	{
		value * axis.x,
		value * axis.y,
		value * axis.z
	};

	Vec3 delta = m_Rotation - rotation;
	m_Rotation = rotation;
	m_Rotation.x = fmodf(m_Rotation.x, ANGLE_360);
	m_Rotation.y = fmodf(m_Rotation.y, ANGLE_360);
	m_Rotation.z = fmodf(m_Rotation.z, ANGLE_360);

	UpdateModelMatrix();

	OnRotationUpdate(delta);
}

void Transform::RotateAroundXAxis(float value)
{
	m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(value), glm::vec3(1.0f, 0.0f, 0.0f));

	m_Rotation.x += value;
	m_Rotation.x = fmodf(m_Rotation.x, ANGLE_360);

	UpdateModelMatrix();

	OnRotationUpdate({ value, 0.0f, 0.0f });
}

void Transform::RotateAroundYAxis(float value)
{
	m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(value), glm::vec3(0.0f, 1.0f, 0.0f));

	m_Rotation.y += value;
	m_Rotation.y = fmodf(m_Rotation.y, ANGLE_360);

	UpdateModelMatrix();

	OnRotationUpdate({ 0.0f, value, 0.0f });
}

void Transform::RotateAroundZAxis(float value)
{
	m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(value), glm::vec3(0.0f, 0.0f, 1.0f));

	m_Rotation.z += value;
	m_Rotation.z = fmodf(m_Rotation.z, ANGLE_360);

	UpdateModelMatrix();

	OnRotationUpdate({ 0.0f, 0.0f, value });
}

void Transform::SetScale(Vec3 scale)
{
	Vec3 delta = m_Scale - scale;
	m_Scale = scale;

	UpdateModelMatrix();

	OnScaleUpdate(delta);
}

void Transform::UpdateModelMatrix()
{
	m_TransformMatrix = glm::scale(m_PositionMatrix * m_RotationMatrix, glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));
}

void Transform::OnPositionUpdate(Vec3 delta)
{
	for (auto& callback : OnPositionUpdateCallbacks)
		callback(delta);
}

void Transform::OnRotationUpdate(Vec3 delta)
{
	for (auto& callback : OnRotationUpdateCallbacks)
		callback(delta);
}

void Transform::OnScaleUpdate(Vec3 delta)
{
	for (auto& callback : OnScaleUpdateCallbacks)
		callback(delta);
}

void Transform::OnInspectorGUI()
{
	ImGui::Text("Tranform:");

	bool dirty = false;

	ImGui::BeginTable("", 3);
	{
		ImGui::TableNextColumn();
		ImGui::TableHeader("X");
		ImGui::TableNextColumn();
		ImGui::TableHeader("Y");
		ImGui::TableNextColumn();
		ImGui::TableHeader("Z");

		ImGui::TableNextRow();
		ImGui::TableNextColumn(); ImGui::PushID(0);
		dirty |= ImGui::DragFloat("", &m_Position.x, 0.01f); ImGui::PopID();
		ImGui::TableNextColumn(); ImGui::PushID(1);
		dirty |= ImGui::DragFloat("", &m_Position.y, 0.01f); ImGui::PopID();
		ImGui::TableNextColumn(); ImGui::PushID(2);
		dirty |= ImGui::DragFloat("", &m_Position.z, 0.01f); ImGui::PopID();
		if (dirty)
		{
			SetPosition(m_Position);
			dirty = false;
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn(); ImGui::PushID(3);
		float val = m_Rotation.x;
		if (ImGui::DragFloat("", &val, 0.01f))
			RotateAroundXAxis(val - m_Rotation.x);
		ImGui::PopID();
		ImGui::TableNextColumn(); ImGui::PushID(4);
		val = m_Rotation.y;
		if (ImGui::DragFloat("", &val, 0.01f))
			RotateAroundYAxis(val - m_Rotation.y);
		ImGui::PopID();
		ImGui::TableNextColumn(); ImGui::PushID(5);
		val = m_Rotation.z;
		if (ImGui::DragFloat("", &val, 0.01f))
			RotateAroundZAxis(val - m_Rotation.z);
		ImGui::PopID();

		ImGui::TableNextRow();
		ImGui::TableNextColumn(); ImGui::PushID(6);
		dirty |= ImGui::DragFloat("", &m_Scale.x, 0.01f); ImGui::PopID();
		ImGui::TableNextColumn(); ImGui::PushID(7);
		dirty |= ImGui::DragFloat("", &m_Scale.y, 0.01f); ImGui::PopID();
		ImGui::TableNextColumn(); ImGui::PushID(8);
		dirty |= ImGui::DragFloat("", &m_Scale.z, 0.01f); ImGui::PopID();
		if (dirty)
		{
			SetScale(m_Scale);
			dirty = false;
		}
	}
	ImGui::EndTable();

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
