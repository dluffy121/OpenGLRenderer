#pragma once

#include <Component/Component.h>
#include "glm/gtc/matrix_transform.hpp"

const std::string CAMERA_TYPE_NAME = "Camera";

class Camera : public Component
{
private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewnMatrix;

	float m_NearClipPlane;
	float m_FarClipPlane;

	bool m_ortho;

	float m_OrthoMultiplier;
	float m_Left;
	float m_Right;
	float m_Bottom;
	float m_Top;

	float m_frustrumWidth;
	float m_frustrumHeight;
	float m_fov;

public:
	Camera();
	Camera(float windowWidth, float windowHeight);
	~Camera();

	inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	inline const glm::mat4& GetViewMatrix() const { return m_ViewnMatrix; }
	inline const glm::mat4& GetVieProjectionwMatrix() const { return m_ProjectionMatrix * GetViewMatrix(); }

	inline bool GetOrtho() { return m_ortho; }
	void SetOrtho(bool value);

	inline float GetNearClipPlane() { return m_NearClipPlane; }
	inline float GetFarClipPlane() { return m_FarClipPlane; }
	void SetNearClipPlane(float nearClipPlane);
	void SetFarClipPlane(float farClipPlane);

	inline float GetOrthoMultiplier() { return m_OrthoMultiplier; }
	inline float GetLeft() { return m_Left; }
	inline float GetRight() { return m_Right; }
	inline float GetBottom() { return m_Bottom; }
	inline float GetTop() { return m_Top; }
	void SetLeft(float left);
	void SetRight(float right);
	void SetBottom(float bottom);
	void SetTop(float top);

	void SetOrthoMultiplier(float orthoMultiplier);

	inline float GetFrustrumWidth() { return m_frustrumWidth; }
	inline float GetFrustrumHeight() { return m_frustrumHeight; }
	inline float GetFOV() { return m_fov; }
	void SetFrustrumWidth(float width);
	void SetFrustrumHeight(float height);
	void SetFOV(float fov);

	void OnInspectorGUI() override;

private:
	void UpdateProjectionMatrix();
	void UpdateViewMatrix();

	void Awake() override;
};