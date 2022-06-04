#pragma once

#include <glm/glm.hpp>
#include "../../Core/Math.h"

class Transform
{
	const float ANGLE_360 = 360.0f;

private:
	core::Vec3 m_Position;
	core::Vec3 m_Rotation;
	core::Vec3 m_Scale;

	glm::mat4 m_PositionMatrix;
	glm::mat4 m_RotationMatrix;
	glm::mat4 m_TransformMatrix;

public:
	Transform();
	~Transform();

	void SetPosition(core::Vec3 position);
	void SetRotation(core::Vec3 rotation);
	void SetScale(core::Vec3 scale);

	inline const core::Vec3& GetPosition() { return m_Position; };
	inline const glm::mat4& GetPositionMatrix() { return m_PositionMatrix; };
	inline const core::Vec3& GetRotation() { return m_Rotation; };
	inline const glm::mat4& GetRotationMatrix() { return m_RotationMatrix; };
	inline const core::Vec3& GetScale() { return m_Scale; };
	inline const glm::mat4& GetTransformMatrix() { return m_TransformMatrix; }

	void OnInspectorGUI();

private:
	void UpdateModelMatrix();
};