#pragma once

#include <GL/glew.h>

namespace core::gl
{
	class IndexBuffer
	{
	public:
		const GLuint Id;
		const unsigned int IndexType;
		const unsigned int Count;

	public:
		IndexBuffer(const unsigned int* data, unsigned int type, unsigned int count);
		IndexBuffer(unsigned int type, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

	private:
		GLuint GenerateBuffer();
	};
}