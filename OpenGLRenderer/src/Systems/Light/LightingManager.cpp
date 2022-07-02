#include "LightingManager.h"

LightingManager::LightingManager()
{}

LightingManager::~LightingManager()
{}

template<class T>
void Subscribe(T* light)
{
	static_assert(falseType<T>::value);
}

template<class T>
void UnSubscribe(T* light)
{
	static_assert(falseType<T>::value);
}

template<>
void LightingManager::Subscribe<DirectionalLight>(DirectionalLight* light)
{
	m_DirectionalLights.push_back(light);
}

template<>
void LightingManager::Subscribe<PointLight>(PointLight* light)
{
	m_PointLights.push_back(light);
}

template<>
void LightingManager::UnSubscribe<DirectionalLight>(DirectionalLight* light)
{
	for (auto i = m_DirectionalLights.begin(); i != m_DirectionalLights.end(); i++)
	{
		if (*i == light)
		{
			auto tmp = *i;
			*i = *(m_DirectionalLights.end() - 1);
			*(m_DirectionalLights.end() - 1) = tmp;
			return;
		}
	}
}

template<>
void LightingManager::UnSubscribe<PointLight>(PointLight* light)
{
	for (auto i = m_PointLights.begin(); i != m_PointLights.end(); i++)
	{
		if (*i == light)
		{
			auto tmp = *i;
			*i = *(m_PointLights.end() - 1);
			*(m_PointLights.end() - 1) = tmp;
			return;
		}
	}
}

void LightingManager::UpdateShaderLightData(Shader& shader, Transform& cameraTransform, Transform& vastuTransform)
{
	for (unsigned int i = 0; i < m_DirectionalLights.size(); i++)
	{
		Light* light = m_DirectionalLights[i];
		light->UpdateShaderLightData(i, shader, cameraTransform, vastuTransform);
	}

	for (unsigned int i = 0; i < m_PointLights.size(); i++)
	{
		Light* light = m_PointLights[i];
		light->UpdateShaderLightData(i, shader, cameraTransform, vastuTransform);
	}
}
