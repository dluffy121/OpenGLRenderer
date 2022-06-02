#include "VertexArray.h"
#include "../../Logger.h"

namespace core::gl
{
	VertexArray::VertexArray() :
		Id(GenerateVAO())
	{}

	VertexArray::~VertexArray()
	{
		GLLog(glDeleteVertexArrays(1, &Id));
		std::cout << "Deleted VertexArray with id: " << Id << std::endl;
	}

	void VertexArray::Bind() const
	{
		GLLog(glBindVertexArray(Id));
	}

	void VertexArray::UnBind() const
	{
		GLLog(glBindVertexArray(0));
	}

	GLuint VertexArray::GenerateVAO()
	{
		GLuint id;
		GLLog(glGenVertexArrays(1, &id));
		return id;
	}
}