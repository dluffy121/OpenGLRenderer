#pragma once

#include "../OpenGLHelper.h"

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