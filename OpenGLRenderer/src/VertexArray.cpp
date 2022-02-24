#include "VertexArray.h"
#include "OpenGLLogger.h"
#include "OpenGLHelper.h"
#include <iostream>

VertexArray::VertexArray() :
	m_isGenerated(false),
	m_VertexArrayId(0)
{}

VertexArray::~VertexArray()
{
	GLLog(glDeleteVertexArrays(1, &m_VertexArrayId));
	std::cout << "Deleted VertexArray with id: " << m_VertexArrayId << std::endl;
}

void VertexArray::Bind()
{
	if (!m_isGenerated)
	{
		GLLog(glGenVertexArrays(1, &m_VertexArrayId));
		m_isGenerated = true;
		for (auto& vbData : m_vbDataCollection)
		{
			const auto& elements = vbData.layout.GetElements();
			unsigned int offset = 0;
			for (unsigned int i = 0; i < elements.size(); i++)
			{
				vbData.layout.DisableElement(i);
			}
		}
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

	//Bind();

	//vbData.vb.Bind();

	//const auto& elements = vbData.layout.GetElements();
	//unsigned int offset = 0;
	//for (unsigned int i = 0; i < elements.size(); i++)
	//{
	//	const auto& element = elements[i];
	//	GLLog(glEnableVertexAttribArray(i));					// enables vertex atrrib array from the bound buffer https://docs.gl/gl4/glEnableVertexAttribArray
	//	glVertexAttribPointer(i,							// define an array vertex attribute data
	//		element.count,
	//		element.type,
	//		element.normalized,
	//		vbData.layout.GetStride(),
	//		(const void*)offset);
	//	offset += element.count * GetSizeOfType(element.type);
	//}
}

void VertexArray::EnableVertexAttribArrays() const
{
	for (auto& vbData : m_vbDataCollection)
	{
		vbData.vb.Bind();
		const auto& elements = vbData.layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			if (element.isEnabled)
				continue;

			GLLog(glEnableVertexAttribArray(i));
			GLLog(glVertexAttribPointer(i,
				element.count,
				element.type,
				element.normalized,
				vbData.layout.GetStride(),
				(const void*)offset));
			offset += element.count * GetSizeOfType(element.type);

			vbData.layout.EnableElement(i);
		}
	}
}