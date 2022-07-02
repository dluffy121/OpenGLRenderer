#pragma once

#include <Light/Light.h>
#include <GameVastu/Transform.h>

class PointLight : public Light
{
public:
	float m_DiffuseIntensity;

public:
	PointLight();
	PointLight(core::Vec3 color, float ambientIntensity, float diffuseIntensity);
	~PointLight();

private:
	virtual void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& cameraTransform, Transform& vastuTransform) override;

	void OnInspectorGUI() override;
};