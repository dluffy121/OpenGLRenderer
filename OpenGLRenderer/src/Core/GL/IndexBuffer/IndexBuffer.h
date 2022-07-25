#pragma once

#include <GL/glew.h>

namespace core::gl
{
	class IndexBuffer
	{
	public:
		const GLuint Id;
		const bool IsStatic;
		const unsigned int IndexType;
		const unsigned int Capacity;

	private:
		bool m_Populated;
		unsigned int m_Count;

	public:
		IndexBuffer(const unsigned int* data, unsigned int type, unsigned int count);
		IndexBuffer(unsigned int type, unsigned int capacity);
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

		void Add(const unsigned int* data, unsigned int count);
		void Remove(unsigned int count);

		inline unsigned int GetCount() { return m_Count; }

	private:
		GLuint GenerateBuffer();
	};
}