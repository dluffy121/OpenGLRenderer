#pragma once

#include "../Component.h"
#include "glm/gtc/matrix_transform.hpp"

class Camera : public Component
{
private:
	glm::mat4 m_ProjectionMatrix;
	float m_OrthoMultiplier;
	float m_NearClipPlane;
	float m_FarClipPlane;
	float m_Left;
	float m_Right;
	float m_Bottom;
	float m_Top;

public:
	Camera();
	Camera(float windowWidth, float windowHeight);
	~Camera();

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	inline float GetOrthoMultiplier() { return m_OrthoMultiplier; }
	inline float GetNearClipPlane() { return m_NearClipPlane; }
	inline float GetFarClipPlane() { return m_FarClipPlane; }
	inline float GetLeft() { return m_Left; }
	inline float GetRight() { return m_Right; }
	inline float GetBottom() { return m_Bottom; }
	inline float GetTop() { return m_Top; }
};