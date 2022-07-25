#pragma once

#include <RenderIntent/RenderResource.h>
#include <RenderIntent/RenderIntent.h>

using namespace core::gl;

RenderResource::RenderResource(unsigned int index, RenderResourceConfig resourceConfig) :
	Index(index)
{
	m_VertexArray = new VertexArray();
	m_VertexArray->Bind();
	m_VertexBuffer = resourceConfig.DynamicVB ? new VertexBuffer(resourceConfig.VBSize) : new VertexBuffer(nullptr, resourceConfig.VBSize);
	m_IndexBuffer = resourceConfig.DynamicIB ? new IndexBuffer(resourceConfig.IBType, resourceConfig.IBCount) : new IndexBuffer(nullptr, resourceConfig.IBType, resourceConfig.IBCount);
	m_VertexBufferLayout = new VertexBufferLayout();
	m_VertexArray->UnBind();
}

RenderResource::RenderResource(unsigned int index, const RenderResource& renderResource) :
	Index(index)
{
	m_VertexArray = renderResource.m_VertexArray;
	m_VertexBuffer = renderResource.m_VertexBuffer;
	m_IndexBuffer = renderResource.m_IndexBuffer;
	m_VertexBufferLayout = renderResource.m_VertexBufferLayout;
}

RenderResource::~RenderResource()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_IndexBuffer;
	delete m_VertexBufferLayout;
}