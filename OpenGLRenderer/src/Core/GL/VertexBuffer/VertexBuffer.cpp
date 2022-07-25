#include "VertexBuffer.h"
#include <Logger/Logger.h>
#include <Math/Math.h>
#include <string>

namespace core::gl
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size) :
		Id(GenerateBuffer()),
		IsStatic(true),
		Capacity(size),
		m_Populated(data != nullptr),
		m_Size(m_Populated ? size : 0)
	{
		Bind();
		GLLog(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));			// update buffer data with its type of usage https://docs.gl/gl4/glBufferData
	}

	VertexBuffer::VertexBuffer(unsigned int capacity) :
		Id(GenerateBuffer()),
		IsStatic(false),
		Capacity(capacity),
		m_Size(0),
		m_Populated(false)
	{
		Bind();
		GLLog(glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, GL_DYNAMIC_DRAW));			// update buffer data with its type of usage https://docs.gl/gl4/glBufferData
	}

	VertexBuffer::~VertexBuffer()
	{
		GLLog(glDeleteBuffers(1, &Id));
		Log("Deleted VertexBuffer with id: " << Id);
	}

	GLuint VertexBuffer::GenerateBuffer()
	{
		GLuint id;
		GLLog(glGenBuffers(1, &id));									// Generate 1 Buffer and get its id in 'bufferId' https://docs.gl/gl4/glGenBuffers
		return id;
	}

	void VertexBuffer::Bind() const
	{
		GLLog(glBindBuffer(GL_ARRAY_BUFFER, Id));						// Bind the generated buffer with its id https://docs.gl/gl4/glBindBuffer
	}

	void VertexBuffer::UnBind() const
	{
		GLLog(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::Add(const void* data, unsigned int size)
	{
		if (IsStatic && m_Populated) return;	// Can only add once if GL_STATIC_DRAW

		ASSERT(m_Size + size <= Capacity,
			"VertexBuffer(" + TO_STRING(Id) + ")::Cannot add more than capacity " + TO_STRING(Capacity) + "\n\tCurrent size:\t" + TO_STRING(m_Size) + "\n\tAdd size:\t" + TO_STRING(size));

		Bind();
		GLLog(glBufferSubData(GL_ARRAY_BUFFER, m_Size, size, data));

		m_Size += size;
		m_Populated = true;
	}

	void VertexBuffer::Remove(unsigned int size)
	{
		if (IsStatic) return;	// Cannot remove if GL_STATIC_DRAW, just delete buffer

		ASSERT(size <= m_Size,
			"VertexBuffer(" + TO_STRING(Id) + ")::Cannot remove more than size " + TO_STRING(m_Size));

		Bind();
		auto offset = m_Size - size;
		GLLog(glBufferSubData(GL_ARRAY_BUFFER, offset, size, nullptr));
		GLLog(glInvalidateBufferSubData(Id, offset, size));

		m_Size -= size;
	}
}