#pragma once

#include <GL/VertexBufferLayout/VertexBufferLayout.h>
#include <GL/IndexBuffer/IndexBuffer.h>
#include <GL/VertexArray/VertexArray.h>
#include <GL/VertexBuffer/VertexBuffer.h>

struct RenderResourceConfig;

class RenderResource
{
public:
	const unsigned int Index;
	core::gl::VertexArray* m_VertexArray;
	core::gl::VertexBuffer* m_VertexBuffer;
	core::gl::IndexBuffer* m_IndexBuffer;
	core::gl::VertexBufferLayout* m_VertexBufferLayout;

public:
	RenderResource(unsigned int index, RenderResourceConfig resourceConfig);
	RenderResource(unsigned int index, const RenderResource& renderResource);
	virtual ~RenderResource() = 0;
};