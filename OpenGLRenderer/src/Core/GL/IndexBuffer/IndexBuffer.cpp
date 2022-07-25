#include "IndexBuffer.h"
#include <GL/OpenGLHelper.h>
#include <iostream>
#include <Logger/Logger.h>

namespace core::gl
{
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int type, unsigned int count) :
		Id(GenerateBuffer()),
		IsStatic(true),
		IndexType(type),
		Capacity(count),
		m_Populated(data != nullptr),
		m_Count(m_Populated ? count : 0)
	{
		Bind();
		GLsizeiptr size = Capacity * GetSizeOfType(type);
		GLLog(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	IndexBuffer::IndexBuffer(unsigned int type, unsigned int capacity) :
		Id(GenerateBuffer()),
		IsStatic(false),
		IndexType(type),
		Capacity(capacity),
		m_Count(0),
		m_Populated(false)
	{
		Bind();
		GLsizeiptr size = Capacity * GetSizeOfType(type);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		GLLog(glDeleteBuffers(1, &Id));
		std::cout << "Deleted IndexBuffer with id: " << Id << std::endl;
	}

	GLuint IndexBuffer::GenerateBuffer()
	{
		GLuint id;
		GLLog(glGenBuffers(1, &id));
		return id;
	}

	void IndexBuffer::Bind() const
	{
		GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id));
	}

	void IndexBuffer::UnBind() const
	{
		GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void IndexBuffer::Add(const unsigned int* data, unsigned int count)
	{
		if (IsStatic && m_Populated) return;	// Can only add once if GL_STATIC_DRAW

		ASSERT(m_Count + count <= Capacity,
			"IndexBuffer(" + TO_STRING(Id) + ")::Cannot add more than Capacity " + TO_STRING(Capacity) + "\n\tCurrent count:\t" + TO_STRING(m_Count) + "\n\tAdd count:\t" + TO_STRING(count));

		Bind();
		auto typeSize = GetSizeOfType(IndexType);
		GLsizeiptr offset = m_Count * typeSize;
		GLsizeiptr size = count * typeSize;
		GLLog(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));

		m_Count += count;
		m_Populated = true;
	}

	void IndexBuffer::Remove(unsigned int count)
	{
		if (IsStatic) return;	// Cannot remove if GL_STATIC_DRAW, just delete buffer

		ASSERT(count <= m_Count,
			"IndexBuffer(" + TO_STRING(Id) + ")::Cannot remove more than Count " + TO_STRING(m_Count));

		Bind();
		auto typeSize = GetSizeOfType(IndexType);
		GLsizeiptr offset = (m_Count - count) * typeSize;
		GLsizeiptr size = count * typeSize;
		GLLog(glInvalidateBufferSubData(Id, offset, size));

		m_Count -= count;
	}
}