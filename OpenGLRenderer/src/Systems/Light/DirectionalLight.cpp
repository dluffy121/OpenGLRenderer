#include "DirectionalLight.h"
#include <imgui/imgui.h>
#include <GameVastu/GameVastu.h>

using namespace core;

DirectionalLight::DirectionalLight() :
	Light(LightType::Directional),
	m_DiffuseIntensity(1.0f),
	m_SpecularIntensity(1.0f)
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().Subscribe<DirectionalLight>(this);
}

DirectionalLight::DirectionalLight(core::Vec3 color, float ambientIntensity, float diffuseIntensity, float specularIntensity) :
	Light(LightType::Directional, color, ambientIntensity),
	m_DiffuseIntensity(diffuseIntensity),
	m_SpecularIntensity(specularIntensity)
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().Subscribe<DirectionalLight>(this);
}

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

void DirectionalLight::UpdateShaderLightData(unsigned int index, Shader& shader, Transform& cameraTransform, Transform& vastuTransform)
{
	std::string uniform = "u_DirLights[" + std::to_string(index) + "]";

	std::string uniformColor = uniform + ".color";
	std::string uniformAmbientIntensity = uniform + ".ambientIntensity";
	std::string uniformDiffuseIntensity = uniform + ".diffuseIntensity";
	std::string uniformSpecularIntensity = uniform + ".specularIntensity";
	std::string uniformDirection = uniform + ".direction";

	auto direction = GetDirection(vastuTransform);

	shader.SetUniform3f(uniformColor, m_Color.x, m_Color.y, m_Color.z);
	shader.SetUniform1f(uniformAmbientIntensity, m_Intensity);
	shader.SetUniform1f(uniformDiffuseIntensity, m_DiffuseIntensity);
	shader.SetUniform1f(uniformSpecularIntensity, m_SpecularIntensity);
	shader.SetUniform3f(uniformDirection, direction.x, direction.y, direction.z);

	// Providing Camera Position in Local Space
	glm::vec3 cameraLocalPos = vastuTransform.GetLocalPosition(cameraTransform);
	shader.SetUniform3f("u_CameraLocalPosition", cameraLocalPos.x, cameraLocalPos.y, cameraLocalPos.z);
}

void DirectionalLight::OnEnable()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().Subscribe<DirectionalLight>(this);
}

void DirectionalLight::OnDisable()
{
	WindowManager::getInstance()->GetCurrentWindow()->GetLightingManager().UnSubscribe<DirectionalLight>(this);
}

void DirectionalLight::OnInspectorGUI()
{
	ImGui::DragFloat("Ambient Intensity", &m_Intensity, 0.01f, 0.0f, 100.0f);
	ImGui::DragFloat("Diffuse Intensity", &m_DiffuseIntensity, 0.01f, 0.0f, 100.0f);
	ImGui::DragFloat("Specular Intensity", &m_SpecularIntensity, 0.01f, 0.0f, 100.0f);

	float color[3] { m_Color.x, m_Color.y, m_Color.z };
	if (ImGui::ColorEdit3("Ambient Color", color))
	{
		m_Color.x = color[0];
		m_Color.y = color[1];
		m_Color.z = color[2];
	}
}
