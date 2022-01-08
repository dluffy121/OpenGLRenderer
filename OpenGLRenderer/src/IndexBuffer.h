#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererId;
	unsigned int m_Count;

public:
	IndexBuffer(const GLuint* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() { return m_Count; }
};