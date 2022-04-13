#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

struct RenderData
{
	const VertexBuffer* m_vb;
	VertexBufferLayout* m_layout;
	const IndexBuffer* m_ib;

	RenderData(const VertexBuffer& vb, VertexBufferLayout& layout, const IndexBuffer& ib) :
		m_vb(&vb),
		m_layout(&layout),
		m_ib(&ib)
	{}
};