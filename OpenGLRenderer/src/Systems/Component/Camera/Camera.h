#pragma once

#include "../Component.h"
#include "glm/gtc/matrix_transform.hpp"

class Camera : public Component
{
public:
	float OrthoMultiplier;

public:
	Camera();
	~Camera();
};