#include "Transform.h"

Transform::Transform() :
	m_Position(glm::vec3(0.0f)),
	m_Rotation(glm::vec3(0.0f)),
	m_Scale(glm::vec3(0.0f)),
	m_modelMatrix(glm::mat4(1))
{}

void Transform::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

void Transform::SetRotation(glm::vec3 rotation)
{
	m_Rotation = rotation;
}

void Transform::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
}

void Transform::UpdateModelMatrix()
{
	m_modelMatrix = glm::translate(glm::mat4(), m_Position);
}
