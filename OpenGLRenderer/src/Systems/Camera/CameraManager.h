#pragma once

#include "Camera.h"
#include <vector>

class CameraManager
{
private:
	std::vector<Camera*> m_Cameras;

public:
	CameraManager();
	~CameraManager();

	void Subscribe(Camera* camera);
	void UnSubscribe(Camera* camera);

	Camera* GetCamera();
};