#include "VertexArray.h"
#include "../../Logger.h"

namespace core::gl
{
	VertexArray::VertexArray() :
		m_VertexArrayObjectId(0)
	{
		GLLog(glGenVertexArrays(1, &m_VertexArrayObjectId));
	}

	VertexArray::~VertexArray()
	{
		GLLog(glDeleteVertexArrays(1, &m_VertexArrayObjectId));
		std::cout << "Deleted VertexArray with id: " << m_VertexArrayObjectId << std::endl;
	}

	void VertexArray::Bind() const
	{
		GLLog(glBindVertexArray(m_VertexArrayObjectId));
	}

	void VertexArray::UnBind() const
	{
		GLLog(glBindVertexArray(0));
	}
}