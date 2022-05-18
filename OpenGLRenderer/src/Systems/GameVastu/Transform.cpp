#include "Transform.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Transform::Transform() :
	m_Position(glm::vec3(0.0f)),
	m_Rotation(glm::vec3(0.0f)),
	m_Scale(glm::vec3(1.0f)),
	m_PositionMatrix(glm::mat4(1)),
	m_RotationMatrix(glm::mat4(1)),
	m_ScaleMatrix(glm::mat4(1)),
	m_modelMatrix(glm::mat4(1))
{}

Transform::~Transform()
{}

void Transform::SetPosition(glm::vec3 position)
{
	m_Position = position;

	m_PositionMatrix = glm::translate(glm::mat4(1), m_Position);

	std::cout << glm::to_string(m_PositionMatrix) << std::endl;

	UpdateModelMatrix();
}

void Transform::SetRotation(glm::vec3 rotation)
{
	if (m_Rotation.x != rotation.x)
	{
		glm::vec3 axis = glm::vec3(0.0f);
		axis.x = 1.0f;
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Rotation.x - rotation.x), axis);
	}
	if (m_Rotation.y != rotation.y)
	{
		glm::vec3 axis = glm::vec3(0.0f);
		axis.y = 1.0f;
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Rotation.y - rotation.y), axis);
	}
	if (m_Rotation.z != rotation.z)
	{
		glm::vec3 axis = glm::vec3(0.0f);
		axis.z = 1.0f;
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Rotation.z - rotation.z), axis);
	}

	m_Rotation = rotation;

	UpdateModelMatrix();
}

void Transform::SetScale(glm::vec3 scale)
{
	m_Scale = scale;

	m_ScaleMatrix = glm::translate(glm::mat4(1), m_Scale);

	std::cout << glm::to_string(m_RotationMatrix) << std::endl;

	UpdateModelMatrix();
}

void Transform::UpdateModelMatrix()
{
	m_modelMatrix = m_PositionMatrix
		* m_RotationMatrix
		* m_ScaleMatrix;
}
