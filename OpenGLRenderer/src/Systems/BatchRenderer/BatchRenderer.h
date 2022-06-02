#pragma once

#include "../../Core/GL/VertexArray/VertexArray.h"
#include "../../Core/GL/VertexBuffer/VertexBuffer.h"
#include "../../Core/GL/IndexBuffer/IndexBuffer.h"
#include "../../Core/GL/VertexBufferLayout/VertexBufferLayout.h"
#include <Shader/Shader.h>

const size_t MAX_VERTEX_COUNT = 1020;

const size_t MAX_TRIANGLE_COUNT = MAX_VERTEX_COUNT / 3;
const size_t MAX_QUAD_COUNT = MAX_VERTEX_COUNT / 4;

const size_t MAX_TRI_INDEX_COUNT = MAX_TRIANGLE_COUNT * 3;
const size_t MAX_QUAD_INDEX_COUNT = MAX_QUAD_COUNT * 6;

class BatchRenderer
{
private:
	const size_t MaxVertexCount = MAX_VERTEX_COUNT;
	const size_t MaxIndexCount = MAX_QUAD_INDEX_COUNT;

	core::gl::VertexArray* m_VetexArray;
	core::gl::VertexBuffer* m_VertexBuffer;
	core::gl::IndexBuffer* m_IndexBuffer;
	core::gl::VertexBufferLayout* m_VBLayout;

	Shader* m_Shader;

	unsigned int m_DrawCount;
	unsigned int m_TotalDrawCalls;

	unsigned int m_VertexCount;
	unsigned int m_IndexCount;
	unsigned long long m_VBOffset;
	unsigned long long m_IBOffset;

public:
	BatchRenderer(unsigned int maxVertexCount = 0, unsigned int maxIndexCount = 0);
	~BatchRenderer();

	inline unsigned int GetDrawCount() { return m_DrawCount; }
	inline unsigned int GetTotalDrawCalls() { return m_TotalDrawCalls; }

	void Reset();
	void Draw();
	void Draw(Shader& shader, core::Vertex*& vertices, unsigned int vCount, unsigned int*& indices, unsigned int iCount);

private:
	void AddVertices(core::Vertex*& vertices, unsigned int count);
	void AddIndices(unsigned int*& indices, unsigned int count);
	void CheckBuffer(unsigned long long vbOffset, unsigned long long ibOffset, unsigned long long vbSize, unsigned long long ibSize) const;
};