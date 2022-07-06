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
	virtual void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform) override;

	void OnDisable() override;
	void OnEnable() override;
};