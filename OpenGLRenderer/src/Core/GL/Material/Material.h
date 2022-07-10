#pragma once

#include <Math/Math.h>
#include <Shader/Shader.h>
#include <GL/Texture/Texture.h>

namespace core::gl
{
	class Material
	{
	public:
		const unsigned int Id;

		Shader* m_Shader;

		Vec3 m_AmbientColor;
		Vec3 m_DiffuseColor;
		Vec3 m_SpecularColor;

		Texture* m_DiffuseTexture;
		Texture* m_SpecularExponent;

	public:
		Material();
		~Material();

		void Bind() const;
		void UnBind() const;
	};
}