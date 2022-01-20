#include "VertexArray.h"
#include "OpenGLLogger.h"

VertexArray::VertexArray()
{
	GLLog(glGenVertexArrays(1, &m_VertexArrayId));
}

VertexArray::~VertexArray()
{
	GLLog(glDeleteVertexArrays(1, &m_VertexArrayId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLLog(glEnableVertexAttribArray(i));					// enables vertex atrrib array from the bound buffer https://docs.gl/gl4/glEnableVertexAttribArray
		GLLog(glVertexAttribPointer(i,							// define an array vertex attribute data
									element.count,
									element.type,
									element.normalized,
									layout.GetStride(),
									(const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLLog(glBindVertexArray(m_VertexArrayId));
}

void VertexArray::UnBind() const
{
	GLLog(glBindVertexArray(0));
}
