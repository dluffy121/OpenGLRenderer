#include <GL/glew.h>
#include "VertexBuffer.h"
#include "OpenGLLogger.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLLog(glGenBuffers(1, &m_VertexBufferId));									// Generate 1 Buffer and get its id in 'bufferId' https://docs.gl/gl4/glGenBuffers
	GLLog(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId));						// Bind the generated buffer with its id https://docs.gl/gl4/glBindBuffer
	GLLog(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));		// update buffer data with its type of usage https://docs.gl/gl4/glBufferData
}

VertexBuffer::~VertexBuffer()
{
	GLLog(glDeleteBuffers(1, &m_VertexBufferId));
}

void VertexBuffer::Bind() const
{
	GLLog(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId));
}

void VertexBuffer::UnBind() const
{
	GLLog(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
