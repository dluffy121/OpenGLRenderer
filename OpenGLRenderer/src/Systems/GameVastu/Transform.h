#pragma once

#include <glm/glm.hpp>
#include "../../Core/Math.h"
#include <functional>

class Transform
{
	const float ANGLE_360 = 360.0f;

public:
	std::vector<std::function<void(core::Vec3)>> OnPositionUpdateCallbacks;
	std::vector<std::function<void(core::Vec3)>> OnRotationUpdateCallbacks;
	std::vector<std::function<void(core::Vec3)>> OnScaleUpdateCallbacks;

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
	void Rotate(core::Vec3 rotation);
	void SetScale(core::Vec3 scale);

	inline const core::Vec3& GetPosition() const { return m_Position; };
	inline const glm::mat4& GetPositionMatrix() const { return m_PositionMatrix; };
	inline const core::Vec3& GetRotation() const { return m_Rotation; };
	inline const glm::mat4& GetRotationMatrix() const { return m_RotationMatrix; };
	inline const core::Vec3& GetScale() const { return m_Scale; };
	inline const glm::mat4& GetTransformMatrix() const { return m_TransformMatrix; }

	void OnInspectorGUI();

private:
	void UpdateModelMatrix();

	void OnPositionUpdate(core::Vec3 delta);
	void OnRotationUpdate(core::Vec3 delta);
	void OnScaleUpdate(core::Vec3 delta);
};