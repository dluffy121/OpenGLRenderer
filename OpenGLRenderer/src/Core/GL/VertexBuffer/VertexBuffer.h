#pragma once

#include <GL/glew.h>

namespace core::gl
{
	class VertexBuffer
	{
	public:
		const GLuint Id;

	public:
		VertexBuffer(const void* data, unsigned int size);
		VertexBuffer(unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;

	private:
		GLuint GenerateBuffer();
	};
}