#pragma once

#include <GL/OpenGLHelper.h>

namespace core::gl
{
	class VertexArray
	{
	public:
		const GLuint Id;

	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void UnBind() const;

	private:
		GLuint GenerateVAO();
	};
}