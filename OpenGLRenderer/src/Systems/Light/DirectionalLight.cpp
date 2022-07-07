#include "DirectionalLight.h"
#include <imgui/imgui.h>
#include <GameVastu/GameVastu.h>

using namespace core;

DirectionalLight::DirectionalLight() :
	Light(LightType::Directional)
{}

DirectionalLight::~DirectionalLight()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().UnSubscribe<DirectionalLight>(this);
}

Vec3 DirectionalLight::GetDirection(Transform& vastuTransform)
{
	auto dirInWorld = gameVastu->m_transform->GetForward();
	glm::mat3 local = glm::transpose(vastuTransform.GetRotationMatrix());
	glm::vec3 dir = glm::normalize(local * dirInWorld);
	return { dir.x, dir.y, dir.z };
}

void DirectionalLight::UpdateShaderLightData(unsigned int index, Shader& shader, Transform& vastuTransform)
{
	char uniform[128];
	memset(uniform, 0, sizeof(uniform));
	snprintf(uniform, sizeof(uniform), "u_DirLights[%d].base.color", index);
	shader.SetUniform3f(uniform, m_Color.x, m_Color.y, m_Color.z);

	snprintf(uniform, sizeof(uniform), "u_DirLights[%d].base.ambientIntensity", index);
	shader.SetUniform1f(uniform, m_AmbientIntensity);

	snprintf(uniform, sizeof(uniform), "u_DirLights[%d].base.diffuseIntensity", index);
	shader.SetUniform1f(uniform, m_DiffuseIntensity);

	snprintf(uniform, sizeof(uniform), "u_DirLights[%d].base.specularIntensity", index);
	shader.SetUniform1f(uniform, m_SpecularIntensity);

	auto direction = GetDirection(vastuTransform);
	snprintf(uniform, sizeof(uniform), "u_DirLights[%d].direction", index);
	shader.SetUniform3f(uniform, direction.x, direction.y, direction.z);
}

void DirectionalLight::OnEnable()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().Subscribe<DirectionalLight>(this);
}

void DirectionalLight::OnDisable()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().UnSubscribe<DirectionalLight>(this);
}