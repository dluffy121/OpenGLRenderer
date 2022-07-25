#pragma once

#include <GL/glew.h>

namespace core::gl
{
	class VertexBuffer
	{
	public:
		const GLuint Id;
		const bool IsStatic;
		const unsigned int Capacity;

	private:
		bool m_Populated;
		unsigned int m_Size;

	public:
		VertexBuffer(const void* data, unsigned int size);
		VertexBuffer(unsigned int capacity);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;

		void Add(const void* data, unsigned int size);
		void Remove(unsigned int size);

		inline unsigned int GetSize() { return m_Size; }

	private:
		GLuint GenerateBuffer();
	};
}