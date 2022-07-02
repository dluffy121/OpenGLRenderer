#include "Light.h"
#include <Window/WindowManager.h>

Light::Light(LightType type) :
	Type(type),
	m_Color(core::Vec3(1.0f)),
	m_Intensity(0.1f)
{
	//WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().Subscribe(this);
}

Light::Light(LightType type, core::Vec3 color, float intensity) :
	Type(type),
	m_Color(color),
	m_Intensity(intensity)
{
	//WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().Subscribe(this);
}

Light::~Light()
{
	//WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().UnSubscribe(this);
}