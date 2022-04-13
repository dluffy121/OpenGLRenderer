#pragma once

#include "Component.h"
#include "glm/gtc/matrix_transform.hpp"

class Camera : public Component
{
public:
	float OrthoMultiplier;

	glm::mat4 m_View;

public:
	Camera();
	~Camera();
};