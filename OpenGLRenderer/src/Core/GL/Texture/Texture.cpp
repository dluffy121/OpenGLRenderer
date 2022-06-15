#include "Texture.h"
#include <GL/glew.h>
#include "stb_image/stb_image.h"
#include <iostream>
#include "../../Logger.h"

namespace core::gl
{
	Texture::Texture(const std::string& texturePath) :
		Id(GenerateTexture()),
		m_FilePath(texturePath),
		m_Width(0),
		m_Height(0),
		m_BPP(0)
	{
		stbi_set_flip_vertically_on_load(1);
		unsigned char* localBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 0);	// 4 is for RGBA

		ASSERT(("Cannot load Image from path: " + texturePath, localBuffer));

		Log("Texture with id: " << Id << std::endl
			<< "Load Status: " << (localBuffer != nullptr) << std::endl
			<< "Resolution: " << m_Width << " x " << m_Height << std::endl
			<< "Bits Per Pixel: " << m_BPP << std::endl);

		GLLog(glBindTexture(GL_TEXTURE_2D, Id));

		GLLog(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));

		GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
		GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

		GLLog(glBindTexture(GL_TEXTURE_2D, 0));

		stbi_image_free(localBuffer);	// we can unload the data, since we just need to show the image and opengl already used the m_LocalBuffer to create texture
	}

	Texture::~Texture()
	{
		GLLog(glDeleteTextures(1, &Id));
	}

	void Texture::Bind(unsigned int slot) const
	{
		ASSERT(glIsTexture(Id) == GL_TRUE);
		GLLog(glActiveTexture(GL_TEXTURE0 + slot));			// since the value is just an integer we can specify slot by just adding
		GLLog(glBindTexture(GL_TEXTURE_2D, Id));
	}

	void Texture::BindToUnit(unsigned int unit) const
	{
		ASSERT(glIsTexture(Id) == GL_TRUE);
		GLLog(glBindTextureUnit(unit, Id));
	}

	void Texture::UnBind() const
	{
		GLLog(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::UnBindFromUnit(unsigned int unit) const
	{
		GLLog(glBindTextureUnit(unit, 0));
	}

	unsigned int Texture::GenerateTexture()
	{
		GLuint id;
		GLLog(glGenTextures(1, &id));
		return id;
	}
}