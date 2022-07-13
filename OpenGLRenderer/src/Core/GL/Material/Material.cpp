#include "Material.h"

namespace core::gl
{
	Material::Material() :
		Id(reinterpret_cast<unsigned int>(this)),
		m_Shader			(NULL),
		m_AmbientColor		(Vec3(1.0f)),
		m_DiffuseColor		(Vec3(1.0f)),
		m_SpecularColor		(Vec3(1.0f)),
		m_DiffuseTexture	(NULL),
		m_SpecularExponent	(NULL)
	{}

	Material::~Material()
	{
		if (m_DiffuseTexture)	delete m_DiffuseTexture;
		if (m_SpecularExponent) delete m_SpecularExponent;
	}

	void Material::Bind() const
	{
		if (m_Shader)
		{
			m_Shader->Bind();
			m_Shader->SetUniform3f("u_Material.ambientColor", m_AmbientColor.x, m_AmbientColor.y, m_AmbientColor.z);
			m_Shader->SetUniform3f("u_Material.diffuseColor", m_DiffuseColor.x, m_DiffuseColor.y, m_DiffuseColor.z);
			m_Shader->SetUniform3f("u_Material.specularColor", m_SpecularColor.x, m_SpecularColor.y, m_SpecularColor.z);
		}

		if (m_DiffuseTexture)	m_DiffuseTexture->BindToUnit(0);
		if (m_SpecularExponent) m_SpecularExponent->BindToUnit(1);
	}

	void Material::UnBind() const
	{
		if (m_Shader) m_Shader->UnBind();

		if (m_DiffuseTexture)	m_DiffuseTexture->UnBindFromUnit(0);
		if (m_SpecularExponent) m_SpecularExponent->UnBindFromUnit(1);
	}
};