#pragma once

#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBufferLayout/VertexBufferLayout.h"

struct VertexBufferData
{
	const VertexBuffer& vb;
	VertexBufferLayout& layout;
};

class VertexArray
{
public:
	bool m_isGenerated;

private:
	unsigned int m_VertexArrayId;
	std::vector<VertexBufferData> m_vbDataCollection;

public:
	VertexArray();
	~VertexArray();

	void Bind();
	void UnBind() const;

	void AddBuffer(const VertexBuffer& vb, VertexBufferLayout& layout);

private:
	void EnableVertexAttribArrays() const;
	void DisableVertexAttribArrays() const;
};