#include "Light.h"
#include <Window/WindowManager.h>

Light::Light(LightType type) :
	Type(type),
	m_Color(core::Vec3(1.0f)),
	m_AmbientIntensity(0.1f),
	m_DiffuseIntensity(1.0f),
	m_SpecularIntensity(1.0f)
{}

Light::~Light()
{}

void Light::OnInspectorGUI()
{
	ImGui::DragFloat("Ambient Intensity", &m_AmbientIntensity, 0.01f, 0.0f, 100.0f);
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