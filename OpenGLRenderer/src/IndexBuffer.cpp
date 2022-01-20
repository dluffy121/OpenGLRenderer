#include <GL/glew.h>
#include "IndexBuffer.h"
#include "OpenGLLogger.h"
#include "OpenGLStaticConsts.h"

IndexBuffer::IndexBuffer(const GLuint* data, unsigned int count)
	: m_Count(count)
{
	GLLog(glGenBuffers(1, &m_IndexBufferId));
	GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId));
	GLLog(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * GLUINT_SIZE, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLLog(glDeleteBuffers(1, &m_IndexBufferId));
}

void IndexBuffer::Bind() const
{
	GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId));
}

void IndexBuffer::UnBind() const
{
	GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
