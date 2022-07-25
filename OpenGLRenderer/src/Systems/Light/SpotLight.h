#pragma once

#include <Light/Light.h>
#include <Light/PointLight.h>
#include <GameVastu/Transform.h>

class SpotLight : public PointLight
{
private:
	float m_CutoffAngle;
	float m_Cutoff;	// Cos(m_CutoffAngle)

public:
	SpotLight();
	~SpotLight();

	core::Vec3 GetLocalDirection(Transform& vastuTransform);
	inline float GetCutOffAngle() { return m_CutoffAngle; }
	void SetCutOffAngle(float angle);

private:
	void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform) override;

	void OnEnable() override;
	void OnDisable() override;
	void OnInspectorGUI() override;
};