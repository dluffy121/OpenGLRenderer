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
		if (m_vb)
			m_vb->Bind();
		if (m_layout)
			m_layout->Bind();
		if (m_ib)
			m_ib->Bind();
	}

	void UnBind()
	{
		if (m_vb)
			m_vb->UnBind();
		if (m_layout)
			m_layout->UnBind();
		if (m_ib)
			m_ib->UnBind();
	}
};
