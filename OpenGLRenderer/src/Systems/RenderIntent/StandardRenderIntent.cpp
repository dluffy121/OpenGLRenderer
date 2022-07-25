#include <Logger/Logger.h>
#include "StandardRenderIntent.h"

using namespace core;

StandardRenderResource::StandardRenderResource(unsigned int index, RenderResourceConfig resourceConfig) :
	RenderResource(index, resourceConfig)
{}

StandardRenderResource::StandardRenderResource(unsigned int index, const StandardRenderResource& renderResource) :
	RenderResource(index, renderResource)
{}

StandardRenderResource::~StandardRenderResource()
{}

unsigned int StandardRenderIntent::GetRenderResource(RenderResourceConfig config)
{
	auto renderResource = new StandardRenderResource(++NextIndex, config);
	m_RenderResources[renderResource->Index] = renderResource;
	return renderResource->Index;
}

void StandardRenderIntent::DeleteRenderResource(unsigned int index)
{
	auto renderResource = m_RenderResources[index];
	if (renderResource) delete renderResource;
	m_RenderResources.erase(index);
}

void StandardRenderIntent::AddVertices(unsigned int index, unsigned int size, const void* data)
{
	m_RenderResources[index]->m_VertexBuffer->Add(data, size);
}

void StandardRenderIntent::RemoveVertices(unsigned int index, unsigned int size)
{
	m_RenderResources[index]->m_VertexBuffer->Remove(size);
}

void StandardRenderIntent::AddIndices(unsigned int index, unsigned int count, unsigned int* data)
{
	m_RenderResources[index]->m_IndexBuffer->Add(data, count);
}

void StandardRenderIntent::RemoveIndices(unsigned int index, unsigned int count)
{
	m_RenderResources[index]->m_IndexBuffer->Remove(count);
}

void StandardRenderIntent::PushIntToLayout(unsigned int index, unsigned int count)
{
	auto renderResource = m_RenderResources[index];
	renderResource->m_VertexBufferLayout->Push<int>(renderResource->m_VertexBuffer->Id, count);
}

void StandardRenderIntent::PushFloatToLayout(unsigned int index, unsigned int count)
{
	auto renderResource = m_RenderResources[index];
	renderResource->m_VertexBufferLayout->Push<float>(renderResource->m_VertexBuffer->Id, count);
}

void StandardRenderIntent::PushVertexToLayout(unsigned int index, unsigned int count)
{
	auto renderResource = m_RenderResources[index];
	renderResource->m_VertexBufferLayout->Push<Vertex>(renderResource->m_VertexBuffer->Id, count);
}

void StandardRenderIntent::BindRenderResource(unsigned int index)
{
	auto renderResource = m_RenderResources[index];
	renderResource->m_VertexArray->Bind();
	renderResource->m_VertexBufferLayout->Enable();
}

void StandardRenderIntent::UnBindRenderResource(unsigned int index)
{
	auto renderResource = m_RenderResources[index];
	renderResource->m_VertexArray->UnBind();
	renderResource->m_VertexBufferLayout->Disable();
}

void StandardRenderIntent::Reset()
{
	DrawCalls = 0;
}

void StandardRenderIntent::Draw()
{}

void StandardRenderIntent::Draw(unsigned int index, core::gl::Material* material, unsigned int indexCount, unsigned int startIndex, unsigned int startVertex)
{
	BindRenderResource(index);
	material->Bind();

	auto l_startIndex = sizeof(unsigned int) * startIndex;

	CheckBuffers(m_RenderResources[index]->m_VertexBuffer->GetSize(), m_RenderResources[index]->m_IndexBuffer->GetCount());

	GLLog(glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)l_startIndex, startVertex));
	DrawCalls++;

	UnBindRenderResource(index);
}