#pragma once

#include <Math/Math.h>
#include <GL/Texture/Texture.h>

namespace core::gl
{
	class Material
	{
	public:
		Vec3 m_AmbientColor;
		Vec3 m_DiffuseColor;
		Vec3 m_SpecularColor;

		Texture* m_DiffuseTexture;
		Texture* m_SpecularExponent;

		// Add Shader

	public:
		Material();
		~Material();
	};
}