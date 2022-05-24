#pragma once

#include "../OpenGLHelper.h"

class VertexArray
{
private:
	GLuint m_VertexArrayObjectId;

public:
	VertexArray();
	~VertexArray();

	inline unsigned int GetId() { return m_VertexArrayObjectId; }

	void Bind() const;
	void UnBind() const;
};