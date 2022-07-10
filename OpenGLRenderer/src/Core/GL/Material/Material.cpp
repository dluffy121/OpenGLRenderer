#include "Material.h"

namespace core::gl
{
	Material::Material() :
		Id(reinterpret_cast<unsigned int>(this)),
		m_Shader(NULL),
		m_AmbientColor(Vec3(0.0f)),
		m_DiffuseColor(Vec3(0.0f)),
		m_SpecularColor(Vec3(0.0f)),
		m_DiffuseTexture(NULL),
		m_SpecularExponent(NULL)
	{}

	Material::~Material()
	{
		if (m_DiffuseTexture) delete m_DiffuseTexture;
		if (m_SpecularExponent) delete m_SpecularExponent;
	}

	void Material::Bind() const
	{
		if (m_DiffuseTexture) m_DiffuseTexture->BindToUnit(0);
		if (m_SpecularExponent) m_SpecularExponent->BindToUnit(1);
	}

	void Material::UnBind() const
	{
		if (m_DiffuseTexture) m_DiffuseTexture->UnBindFromUnit(0);
		if (m_SpecularExponent) m_SpecularExponent->UnBindFromUnit(1);
	}
};