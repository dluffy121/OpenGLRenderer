#include "SpotLight.h"
#include <imgui/imgui.h>
#include <Window/WindowManager.h>
#include <GameVastu/GameVastu.h>

SpotLight::SpotLight() :
	PointLight(LightType::Spot),
	m_CutoffAngle(10.0f)
{
	SetCutOffAngle(m_CutoffAngle);
}

SpotLight::~SpotLight()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().UnSubscribe<SpotLight>(this);
}

core::Vec3 SpotLight::GetDirection(Transform& vastuTransform)
{
	auto dirInWorld = gameVastu->m_transform->GetForward();
	glm::mat3 local = glm::transpose(vastuTransform.GetRotationMatrix());
	glm::vec3 dir = glm::normalize(local * dirInWorld);
	return { dir.x, dir.y, dir.z };
}

void SpotLight::SetCutOffAngle(float angle)
{
	m_CutoffAngle = angle;
	m_Cutoff = glm::cos( glm::radians(m_CutoffAngle));
}

void SpotLight::UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform)
{
	char uniform[128];
	memset(uniform, 0, sizeof(uniform));
	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].base.base.color", index);
	shader.SetUniform3f(uniform, m_Color.x, m_Color.y, m_Color.z);

	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].base.base.ambientIntensity", index);
	shader.SetUniform1f(uniform, m_AmbientIntensity);

	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].base.base.diffuseIntensity", index);
	shader.SetUniform1f(uniform, m_DiffuseIntensity);

	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].base.base.specularIntensity", index);
	shader.SetUniform1f(uniform, m_SpecularIntensity);

	auto localPos = vastuTransform.GetLocalPosition(*gameVastu->m_transform);
	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].base.localPos", index);
	shader.SetUniform3f(uniform, localPos.x, localPos.y, localPos.z);

	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].base.atten.Constant", index);
	shader.SetUniform1f(uniform, m_ConstantAttenuation);

	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].base.atten.Linear", index);
	shader.SetUniform1f(uniform, m_LinearAttenuation);

	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].base.atten.Exponent", index);
	shader.SetUniform1f(uniform, m_ExponentialAttenuation);

	auto direction = GetDirection(vastuTransform);
	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].direction", index);
	shader.SetUniform3f(uniform, direction.x, direction.y, direction.z);

	snprintf(uniform, sizeof(uniform), "u_SpotLights[%d].cutoff", index);
	shader.SetUniform1f(uniform, m_Cutoff);
}

void SpotLight::OnEnable()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().Subscribe<SpotLight>(this);
}

void SpotLight::OnDisable()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().UnSubscribe<SpotLight>(this);
}

void SpotLight::OnInspectorGUI()
{
	PointLight::OnInspectorGUI();

	if (ImGui::DragFloat("Cutoff", &m_CutoffAngle, 0.01f, 0.0f, 90.0f))
		SetCutOffAngle(m_CutoffAngle);
}
