#pragma once
#include <string>

namespace core::gl
{
	class Texture
	{
	public:
		const unsigned int Id;
		const std::string m_FilePath;

	private:
		int m_Width, m_Height, m_BPP;

	public:
		Texture(const std::string& texturePath);
		~Texture();

		void Bind(unsigned int slot = 0) const;		// number of slots depends on device, could generally range from 8 to 32
		void BindToUnit(unsigned int unit) const;
		void UnBind() const;
		void UnBindFromUnit(unsigned int unit) const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

	private:
		unsigned int GenerateTexture();
	};
}