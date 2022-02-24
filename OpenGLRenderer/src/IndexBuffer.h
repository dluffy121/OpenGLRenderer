#pragma once

class IndexBuffer
{
private:
	unsigned int m_IndexBufferId;
	unsigned int m_IndexType;
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* data, unsigned int type, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetIndexType() const { return m_IndexType; }
	inline unsigned int GetCount() const { return m_Count; }
};