#pragma once

#include <vector>
#include <GL/glew.h>
#include "../../Logger.h"
#include "../OpenGLHelper.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	bool isEnabled;
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout() :
		m_Stride(0)
	{}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(falseType<T>::value);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
	void Bind();
	void UnBind();

private:
	inline void EnableElement(int index) { m_Elements[index].isEnabled = true; }
	inline void DisableElement(int index) { m_Elements[index].isEnabled = false; }
};

template<typename T>
struct falseType : std::false_type { };