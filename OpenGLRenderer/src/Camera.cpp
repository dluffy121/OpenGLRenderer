#include "Camera.h"

Camera::Camera():
	OrthoMultiplier(1.0f),
	m_View(glm::mat4(1.0f))
{}

Camera::~Camera()
{}