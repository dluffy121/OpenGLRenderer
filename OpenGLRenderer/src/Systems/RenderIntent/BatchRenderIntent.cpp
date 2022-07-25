#include "BatchRenderIntent.h"
#include <Logger/Logger.h>

using namespace core;

BatchRenderResource::BatchRenderResource(unsigned int index, RenderResourceConfig resourceConfig) :
	RenderResource(index, resourceConfig),
	m_VBOffset(Index == 0 ? 0 : -1),
	m_VBSize(0),
	m_IBOffset(Index == 0 ? 0 : -1),
	m_IBCount(0)
{}

BatchRenderResource::BatchRenderResource(unsigned int index, const BatchRenderResource& renderResource) :
	RenderResource(index, renderResource),
	m_VBOffset(renderResource.m_VBSize),
	m_VBSize(0),
	m_IBOffset(renderResource.m_IBCount),
	m_IBCount(0)
{}

BatchRenderResource::~BatchRenderResource()
{}

unsigned int BatchRenderIntent::GetRenderResource(RenderResourceConfig resourceConfig)
{
	BatchRenderResource* batchRenderResource;
	if (m_RenderResources.empty())
	{
		resourceConfig.DynamicVB = true;
		resourceConfig.VBSize = MAX_VERTEX_COUNT * sizeof(Vertex);
		resourceConfig.DynamicIB = true;
		resourceConfig.IBType = GL_UNSIGNED_INT;
		resourceConfig.IBCount = MAX_INDEX_COUNT;
		batchRenderResource = new BatchRenderResource(++NextIndex, resourceConfig);
		batchRenderResource->m_VertexBufferLayout->Push<Vertex>(batchRenderResource->m_VertexBuffer->Id, 1);
	}
	else
	{
		auto lastBatchRenderResource = dynamic_cast<BatchRenderResource*>((*m_RenderResources.rbegin()).second);
		batchRenderResource = new BatchRenderResource(++NextIndex, *lastBatchRenderResource);	// To reference already created resources
	}

	m_RenderResources[batchRenderResource->Index] = batchRenderResource;

	return batchRenderResource->Index;
}

void BatchRenderIntent::DeleteRenderResource(unsigned int index)
{
}

void BatchRenderIntent::AddVertices(unsigned int index, unsigned int size, const void* data)
{
	auto batchRenderResource = dynamic_cast<BatchRenderResource*>(m_RenderResources[index]);
	batchRenderResource->m_VBSize += size;
	if (index > 1)
	{
		auto lastBatchRenderResource = dynamic_cast<BatchRenderResource*>(m_RenderResources[--index]);
		batchRenderResource->m_VBOffset = lastBatchRenderResource->m_VBSize;
	}

	//unsigned int vbOffset = 0;
	//for (auto renderResource : m_RenderResources)
	//{
	//	auto l_renderResource = dynamic_cast<BatchRenderResource*>(m_RenderResources[index]);
	//	l_renderResource->m_VBOffset = vbOffset;

	//	vbOffset += l_renderResource->m_VBSize;
	//}
	batchRenderResource->m_VertexBuffer->Add(data, size);
}

void BatchRenderIntent::RemoveVertices(unsigned int index, unsigned int size) {}

void BatchRenderIntent::AddIndices(unsigned int index, unsigned int count, unsigned int* data)
{
	auto batchRenderResource = dynamic_cast<BatchRenderResource*>(m_RenderResources[index]);
	batchRenderResource->m_IBCount += count;
	if (index > 1)
	{
		auto lastBatchRenderResource = dynamic_cast<BatchRenderResource*>(m_RenderResources[--index]);
		batchRenderResource->m_IBOffset = lastBatchRenderResource->m_IBCount;
	}

	if(batchRenderResource->m_VBOffset > 0)
	{
		auto indexIncrement = batchRenderResource->m_VBOffset / sizeof(Vertex);
		for (int i = 0; i < count; i++)
			data[i] += indexIncrement;
	}

	//unsigned int ibOffset = 0;
	//for (auto renderResource : m_RenderResources)
	//{
	//	auto l_renderResource = dynamic_cast<BatchRenderResource*>(m_RenderResources[index]);
	//	l_renderResource->m_IBOffset = ibOffset;

	//	ibOffset += l_renderResource->m_IBCount;
	//}


	batchRenderResource->m_IndexBuffer->Add(data, count);
}

void BatchRenderIntent::RemoveIndices(unsigned int index, unsigned int count) {}

void BatchRenderIntent::PushIntToLayout(unsigned int index, unsigned int count) {}

void BatchRenderIntent::PushFloatToLayout(unsigned int index, unsigned int count) {}

void BatchRenderIntent::PushVertexToLayout(unsigned int index, unsigned int count) {}

void BatchRenderIntent::BindRenderResource(unsigned int index)
{
	auto renderResource = m_RenderResources[index];
	renderResource->m_VertexArray->Bind();
	renderResource->m_VertexBufferLayout->Enable();
}

void BatchRenderIntent::UnBindRenderResource(unsigned int index)
{
	auto renderResource = m_RenderResources[index];
	renderResource->m_VertexArray->UnBind();
	renderResource->m_VertexBufferLayout->Disable();
}

void BatchRenderIntent::Reset()
{
	DrawCalls = 0;
	m_CurrentMaterial = nullptr;
	m_StartVertex = 0;
	m_StartIndex = 0;
	m_IndexCount = 0;

	// Flush all Dynamic data
	// As it is assumed that it is updated every Frame
	for (auto resource : m_RenderResources)
	{
		auto renderResource = dynamic_cast<BatchRenderResource*>(resource.second);
		if (!renderResource->m_VertexBuffer->IsStatic)
			renderResource->m_VertexBuffer->Remove(renderResource->m_VertexBuffer->GetSize());
		if (!renderResource->m_IndexBuffer->IsStatic)
			renderResource->m_IndexBuffer->Remove(renderResource->m_IndexBuffer->GetCount());
		renderResource->m_VBSize = 0;
		renderResource->m_VBOffset = 0;
		renderResource->m_IBCount = 0;
		renderResource->m_IBOffset = 0;
	}
}

void BatchRenderIntent::Draw()
{
	if (!m_CurrentMaterial) return;

	BindRenderResource(1);

	m_CurrentMaterial->Bind();

	CheckBuffers(m_RenderResources[1]->m_VertexBuffer->GetSize(), m_RenderResources[1]->m_IndexBuffer->GetCount());

	GLLog(glDrawElementsBaseVertex(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * m_StartIndex), m_StartVertex));
	DrawCalls++;

	m_CurrentMaterial->UnBind();

	UnBindRenderResource(1);
}

void BatchRenderIntent::Draw(unsigned int index, core::gl::Material* material, unsigned int indexCount, unsigned int startIndex, unsigned int startVertex)
{
	//material->Bind();

	//auto renderResource = dynamic_cast<BatchRenderResource*>(m_RenderResources[index]);

	//startVertex += renderResource->m_VBOffset / sizeof(core::Vertex);
	//startIndex += renderResource->m_IBOffset;
	//auto l_startIndex = sizeof(unsigned int) * startIndex;

	//CheckBuffers(renderResource->m_VertexBuffer->GetSize(), renderResource->m_IndexBuffer->GetCount());

	//GLLog(glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)l_startIndex, startVertex));
	//DrawCalls++;

	if (m_CurrentMaterial && m_CurrentMaterial->Id == material->Id)
	{
		m_IndexCount += indexCount;
	}
	else
	{
		Draw();

		m_CurrentMaterial = material;

		auto renderResource = dynamic_cast<BatchRenderResource*>(m_RenderResources[index]);
		m_IndexCount = indexCount;
		m_StartVertex = startVertex + (renderResource->m_VBOffset / sizeof(core::Vertex));
		m_StartIndex = startIndex + renderResource->m_IBOffset;
	}
}