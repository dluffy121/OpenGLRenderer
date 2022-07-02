#pragma once

#include <Light/Light.h>
#include <GameVastu/Transform.h>

class DirectionalLight : public Light
{
public:
	float m_DiffuseIntensity;
	float m_SpecularIntensity;

public:
	DirectionalLight();
	DirectionalLight(core::Vec3 color, float ambientIntensity, float diffuseIntensity, float specularIntensity);
	~DirectionalLight();

	core::Vec3 GetDirection(Transform& vastuTransform);

private:
	virtual void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& cameraTransform, Transform& vastuTransform) override;

	void OnDisable() override;
	void OnInspectorGUI() override;
};