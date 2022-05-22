#pragma once
#include <string>

class Texture
{
private:
	unsigned int m_TextureId;
	std::string m_FilePath;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& texturePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;		// number of slots depends on device, could generally range from 8 to 32
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};