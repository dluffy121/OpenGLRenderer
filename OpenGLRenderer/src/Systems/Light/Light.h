#pragma once

#include <Component/Component.h>
#include <Math/Math.h>
#include <Shader/Shader.h>
#include <GameVastu/Transform.h>

enum LightType
{
	Directional = 0,
	Point = 1,
	Spot = 2
};

class Light : public Component
{
public:
	const LightType Type;
	core::Vec3 m_Color = core::Vec3(1.0f);
	float m_AmbientIntensity = 0.1f;
	float m_DiffuseIntensity = 1.0f;
	float m_SpecularIntensity = 1.0f;

public:
	Light(LightType type);
	~Light();

	virtual void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform) {}

protected:
	virtual void OnInspectorGUI() override;
};

template<class T>
concept Light_T = requires
{
	std::is_base_of<Light, T>::value == true && std::is_pointer<T>::value == true;
};

template<class T>
concept Light_T_ptr = requires
{
	std::is_base_of<Light*, T*>::value == true && std::is_pointer<T*>::value == true;
};