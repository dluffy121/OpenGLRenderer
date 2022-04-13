#pragma once
#include "glm/gtc/matrix_transform.hpp"

class Transform
{
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	glm::mat4 m_modelMatrix;

public:
	Transform();
	~Transform();

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	inline glm::vec3 GetPosition() { return m_Position; };
	inline glm::vec3 GetRotation() { return m_Rotation; };
	inline glm::vec3 GetScale() { return m_Scale; };
	inline glm::mat4 GetModelMatrix() { return m_modelMatrix; }

private:
	void UpdateModelMatrix();
};