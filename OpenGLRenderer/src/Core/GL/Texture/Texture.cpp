#include "Texture.h"
#include <GL/glew.h>
#include "stb_image/stb_image.h"
#include <iostream>
#include "../../Logger.h"

Texture::Texture(const std::string& texturePath) :
	m_TextureId(0),
	m_FilePath(texturePath),
	m_Width(0),
	m_Height(0),
	m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	unsigned char* localBuffer; = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 0);	// 4 is for RGBA

	Log("Texture with id: " << m_TextureId << std::endl
		<< "Load Status: " << (localBuffer != nullptr) << std::endl
		<< "Resolution: " << m_Width << " x " << m_Height << std::endl
		<< "Bits Per Pixel: " << m_BPP << std::endl);

	GLLog(glGenTextures(1, &m_TextureId));
	GLLog(glBindTexture(GL_TEXTURE_2D, m_TextureId));

	GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLLog(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));

	GLLog(glBindTexture(GL_TEXTURE_2D, 0));

	if (localBuffer)					// we can unload the data, since we just need to show the image and opengl already used the m_LocalBuffer to create texture
		stbi_image_free(localBuffer);
}

Texture::~Texture()
{
	GLLog(glDeleteTextures(1, &m_TextureId));
}

void Texture::Bind(unsigned int slot) const
{
	ASSERT(glIsTexture(m_TextureId) == GL_TRUE);
	GLLog(glActiveTexture(GL_TEXTURE0 + slot));			// since the value is just an integer we can specify slot by just adding
	GLLog(glBindTexture(GL_TEXTURE_2D, m_TextureId));
}

void Texture::UnBind() const
{
	GLLog(glBindTexture(GL_TEXTURE_2D, 0));
}