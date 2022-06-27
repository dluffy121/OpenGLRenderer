#include "IndexBuffer.h"
#include <GL/OpenGLHelper.h>
#include <iostream>
#include <Logger/Logger.h>

namespace core::gl
{
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int type, unsigned int count) :
		Id(GenerateBuffer()),
		IndexType(type),
		Count(count)
	{
		Bind();
		GLsizeiptr size = count * GetSizeOfType(type);
		GLLog(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	IndexBuffer::IndexBuffer(unsigned int type, unsigned int count) :
		Id(GenerateBuffer()),
		IndexType(type),
		Count(count)
	{
		Bind();
		GLsizeiptr size = count * GetSizeOfType(type);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		GLLog(glDeleteBuffers(1, &Id));
		std::cout << "Deleted IndexBuffer with id: " << Id << std::endl;
	}

	void IndexBuffer::Bind() const
	{
		GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id));
	}

	void IndexBuffer::UnBind() const
	{
		GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	GLuint IndexBuffer::GenerateBuffer()
	{
		GLuint id;
		GLLog(glGenBuffers(1, &id));
		return id;
	}
}