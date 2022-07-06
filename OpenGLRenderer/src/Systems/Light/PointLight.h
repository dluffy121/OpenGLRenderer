#pragma once

#include <Light/Light.h>
#include <GameVastu/Transform.h>

class PointLight : public Light
{
public:
	float m_ConstantAttenuation;
	float m_LinearAttenuation;
	float m_ExponentialAttenuation;

public:
	PointLight();
	~PointLight();

private:
	virtual void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform) override;

	void OnEnable();
	void OnDisable();
	void OnInspectorGUI() override;
};