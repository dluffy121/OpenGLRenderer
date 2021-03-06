#pragma once

#include <Light/Light.h>
#include <GameVastu/Transform.h>

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	core::Vec3 GetDirection(Transform& vastuTransform);

private:
	void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform) override;

	void OnEnable() override;
	void OnDisable() override;
};