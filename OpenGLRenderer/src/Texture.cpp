#include "Texture.h"
#include "OpenGLLogger.h"
#include <GL/glew.h>
#include "vendor/stb_image/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& texturePath) :
	m_TextureId(0),
	m_FilePath(texturePath),
	m_LocalBuffer(nullptr),
	m_width(0),
	m_height(0),
	m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_width, &m_height, &m_BPP, 0);	// 4 is for RGBA

	Log("Texture with id: " << m_TextureId << std::endl
		<< "Load Status: " << (m_LocalBuffer != nullptr) << std::endl
		<< "Resolution: " << m_width << " x " << m_height << std::endl
		<< "Bits Per Pixel: " << m_BPP << std::endl);

	GLLog(glGenTextures(1, &m_TextureId));
	GLLog(glBindTexture(GL_TEXTURE_2D, m_TextureId));

	GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLLog(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLLog(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	GLLog(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)					// we can unload the data, since we just need to show the image and opengl already used the m_LocalBuffer to create texture
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	// free local buffer is data was needed for other purposes like sampling
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

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