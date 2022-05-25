#pragma once

#include "../../../Core/GL/VertexBuffer/VertexBuffer.h"
#include "../../../Core/GL/VertexBufferLayout/VertexBufferLayout.h"
#include "../../../Core/GL/IndexBuffer/IndexBuffer.h"

using namespace core::gl;

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

	void Bind()
	{
		m_vb->Bind();
		m_layout->Bind();
		m_ib->Bind();
	}

	void UnBind()
	{
		m_vb->UnBind();
		m_layout->UnBind();
		m_ib->UnBind();
	}
};
