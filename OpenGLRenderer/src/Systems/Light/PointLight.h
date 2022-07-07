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

protected:
	PointLight(LightType type);

	virtual void OnInspectorGUI() override;

private:
	void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform) override;

	void OnEnable() override;
	void OnDisable() override;
};