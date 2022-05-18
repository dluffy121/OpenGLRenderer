#include "VertexArray.h"
#include "../../Logger.h"
#include "../OpenGLHelper.h"

VertexArray::VertexArray() :
	m_isGenerated(false),
	m_VertexArrayId(0)
{}

VertexArray::~VertexArray()
{
	if (m_isGenerated)
	{
		GLLog(glDeleteVertexArrays(1, &m_VertexArrayId));
		std::cout << "Deleted VertexArray with id: " << m_VertexArrayId << std::endl;
	}
}

void VertexArray::Bind()
{
	if (!m_isGenerated)
	{
		GLLog(glGenVertexArrays(1, &m_VertexArrayId));
		m_isGenerated = true;
		DisableVertexAttribArrays();
	}

	GLLog(glBindVertexArray(m_VertexArrayId));
	EnableVertexAttribArrays();
}

void VertexArray::UnBind() const
{
	GLLog(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, VertexBufferLayout& layout)
{
	m_vbDataCollection.push_back({ vb, layout });
}

void VertexArray::EnableVertexAttribArrays() const
{
	for (auto& vbData : m_vbDataCollection)
	{
		vbData.vb.Bind();
		vbData.layout.Bind();
	}
}

void VertexArray::DisableVertexAttribArrays() const
{
	for (auto& vbData : m_vbDataCollection)
	{
		vbData.layout.UnBind();
	}
}