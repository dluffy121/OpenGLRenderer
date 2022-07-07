#include "PointLight.h"
#include <imgui/imgui.h>
#include <Window/WindowManager.h>
#include <GameVastu/GameVastu.h>
#include <GameVastu/Transform.h>

PointLight::PointLight() :
	Light(LightType::Point),
	m_ConstantAttenuation(1.0f),
	m_LinearAttenuation(0.5f),
	m_ExponentialAttenuation(0.5f)
{}

PointLight::PointLight(LightType type) :
	Light(type),
	m_ConstantAttenuation(1.0f),
	m_LinearAttenuation(0.5f),
	m_ExponentialAttenuation(0.5f)
{}

PointLight::~PointLight()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().UnSubscribe<PointLight>(this);
}

void PointLight::UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform)
{
	char uniform[128];
	memset(uniform, 0, sizeof(uniform));
	snprintf(uniform, sizeof(uniform), "u_PointLights[%d].base.color", index);
	shader.SetUniform3f(uniform, m_Color.x, m_Color.y, m_Color.z);

	snprintf(uniform, sizeof(uniform), "u_PointLights[%d].base.ambientIntensity", index);
	shader.SetUniform1f(uniform, m_AmbientIntensity);

	snprintf(uniform, sizeof(uniform), "u_PointLights[%d].base.diffuseIntensity", index);
	shader.SetUniform1f(uniform, m_DiffuseIntensity);

	snprintf(uniform, sizeof(uniform), "u_PointLights[%d].base.specularIntensity", index);
	shader.SetUniform1f(uniform, m_SpecularIntensity);

	auto localPos = vastuTransform.GetLocalPosition(*gameVastu->m_transform);
	snprintf(uniform, sizeof(uniform), "u_PointLights[%d].localPos", index);
	shader.SetUniform3f(uniform, localPos.x, localPos.y, localPos.z);

	snprintf(uniform, sizeof(uniform), "u_PointLights[%d].atten.Constant", index);
	shader.SetUniform1f(uniform, m_ConstantAttenuation);

	snprintf(uniform, sizeof(uniform), "u_PointLights[%d].atten.Linear", index);
	shader.SetUniform1f(uniform, m_LinearAttenuation);

	snprintf(uniform, sizeof(uniform), "u_PointLights[%d].atten.Exponent", index);
	shader.SetUniform1f(uniform, m_ExponentialAttenuation);
}

void PointLight::OnEnable()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().Subscribe<PointLight>(this);
}

void PointLight::OnDisable()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().UnSubscribe<PointLight>(this);
}

void PointLight::OnInspectorGUI()
{
	Light::OnInspectorGUI();

	ImGui::DragFloat("Constant Atten", &m_ConstantAttenuation, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat("Linear Atten", &m_LinearAttenuation, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat("Exponential Atten", &m_ExponentialAttenuation, 0.01f, -100.0f, 100.0f);
}
