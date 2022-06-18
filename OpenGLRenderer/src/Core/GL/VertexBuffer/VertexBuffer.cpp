#include "VertexBuffer.h"
#include <Logger/Logger.h>
#include <Math/Math.h>

namespace core::gl
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size) :
		Id(GenerateBuffer())
	{
		Bind();
		GLLog(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));			// update buffer data with its type of usage https://docs.gl/gl4/glBufferData
	}

	VertexBuffer::VertexBuffer(unsigned int size) :
		Id(GenerateBuffer())
	{
		Bind();
		GLLog(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));			// update buffer data with its type of usage https://docs.gl/gl4/glBufferData
	}

	VertexBuffer::~VertexBuffer()
	{
		GLLog(glDeleteBuffers(1, &Id));
		std::cout << "Deleted VertexBuffer with id: " << Id << std::endl;
	}

	void VertexBuffer::Bind() const
	{
		GLLog(glBindBuffer(GL_ARRAY_BUFFER, Id));						// Bind the generated buffer with its id https://docs.gl/gl4/glBindBuffer
	}

	void VertexBuffer::UnBind() const
	{
		GLLog(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	GLuint VertexBuffer::GenerateBuffer()
	{
		GLuint id;
		GLLog(glGenBuffers(1, &id));									// Generate 1 Buffer and get its id in 'bufferId' https://docs.gl/gl4/glGenBuffers
		return id;
	}
}