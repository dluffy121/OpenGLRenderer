#include "Material.h"

namespace core::gl
{
	Material::Material() :
		m_AmbientColor(Vec3(1.0f)),
		m_DiffuseColor(Vec3(1.0f)),
		m_DiffuseTexture(NULL),
		m_SpecularExponent(NULL)
	{}

	Material::~Material()
	{
		if (m_DiffuseTexture) delete m_DiffuseTexture;
		if (m_SpecularExponent) delete m_SpecularExponent;
	}
};