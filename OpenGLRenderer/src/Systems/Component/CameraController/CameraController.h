#pragma once

#include <Component/Component.h>

class CameraController : public Component
{
private:
	float m_delta;
	float m_MoveSpeed;
	float m_ZoomSpeed;

public:
	CameraController();
	~CameraController();

private:
	virtual void Update() override;
	virtual void OnInspectorGUI() override;
};