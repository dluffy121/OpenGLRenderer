#include <GL/glew.h>
#include "IndexBuffer.h"
#include "OpenGLLogger.h"
#include "OpenGLHelper.h"
#include <iostream>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int type, unsigned int count) :
	m_IndexType(type),
	m_Count(count)
{
	GLLog(glGenBuffers(1, &m_IndexBufferId));
	GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId));
	unsigned int size = count * GetSizeOfType(type);
	GLLog(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * size, data, GL_STATIC_DRAW));
	UnBind();
}

IndexBuffer::~IndexBuffer()
{
	GLLog(glDeleteBuffers(1, &m_IndexBufferId));
	std::cout << "Deleted IndexBuffer with id: " << m_IndexBufferId << std::endl;
}

void IndexBuffer::Bind() const
{
	GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId));
}

void IndexBuffer::UnBind() const
{
	GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
