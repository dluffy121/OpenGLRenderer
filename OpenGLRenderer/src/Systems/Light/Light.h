#pragma once

#include <Component/Component.h>
#include <Math/Math.h>
#include <Shader/Shader.h>
#include <GameVastu/Transform.h>

enum LightType
{
	Directional = 0,
	Point = 1
};

class Light : public Component
{
public:
	const LightType Type;

public:
	core::Vec3 m_Color = core::Vec3(1.0f);
	float m_Intensity = 1.0f;
	bool Dirty;

	Light(LightType type);
	Light(LightType type, core::Vec3 color, float intensity);
	~Light();

	virtual void UpdateShaderLightData(unsigned int index, Shader& shader, Transform& cameraTransform, Transform& vastuTransform) {}
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