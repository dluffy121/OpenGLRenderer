#pragma once

#include "Light.h"
#include <vector>
#include "DirectionalLight.h"
#include "PointLight.h"

class LightingManager
{
private:
	std::vector<DirectionalLight*> m_DirectionalLights;
	std::vector<PointLight*> m_PointLights;

public:
	LightingManager();
	~LightingManager();

	template<class T>
	void Subscribe(T* light);

	template<class T>
	void UnSubscribe(T* light);

	void UpdateShaderLightData(Shader& shader, Transform& cameraTransform, Transform& vastuTransform);

	// TODO: Light Local Direction Calculation
	// Shader Updation
};