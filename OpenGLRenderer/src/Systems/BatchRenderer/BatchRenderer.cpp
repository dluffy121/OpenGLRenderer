#include "BatchRenderer.h"
#include "../../Core/Logger.h"

using namespace core;
using namespace core::gl;

BatchRenderer::BatchRenderer(unsigned int maxVertexCount, unsigned int maxIndexCount) :
	MaxVertexCount(maxVertexCount == 0 ? MAX_VERTEX_COUNT : maxVertexCount),
	MaxIndexCount(maxIndexCount == 0 ? MAX_QUAD_INDEX_COUNT : maxIndexCount),
	m_Shader(NULL),
	m_VertexCount(0),
	m_IndexCount(0),
	m_VBOffset(0),
	m_IBOffset(0),
	m_DrawCount(0),
	m_TotalDrawCalls(0)
{
	m_VetexArray = new VertexArray();
	m_VetexArray->Bind();
	m_VertexBuffer = new VertexBuffer(MaxVertexCount * sizeof(Vertex));
	m_VertexBuffer->Bind();
	m_IndexBuffer = new IndexBuffer(GL_UNSIGNED_INT, MaxIndexCount);
	m_IndexBuffer->Bind();
	m_VBLayout = new VertexBufferLayout();
	m_VBLayout->Push<Vertex>(m_VertexBuffer->Id, 1);
	m_VBLayout->Bind();
}

BatchRenderer::~BatchRenderer()
{
	delete m_VBLayout;
	delete m_IndexBuffer;
	delete m_VertexBuffer;
	delete m_VetexArray;
}

void BatchRenderer::Reset()
{
	m_DrawCount = m_TotalDrawCalls = 0;
	m_Shader = NULL;

	m_VetexArray->Bind();
}

void BatchRenderer::Draw()
{
	//CheckBuffer(0, 0, m_VertexCount * sizeof(Vertex), m_IndexCount * sizeof(unsigned int));

	m_DrawCount++;
	GLLog(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr));	// this method will draw from binded element buffer array https://docs.gl/gl4/glDrawElements

	m_VertexCount = m_IndexCount = 0;
	m_VBOffset = m_IBOffset = 0;
}

void BatchRenderer::Draw(Shader& shader, Vertex*& vertices, unsigned int vCount, unsigned int*& indices, unsigned int iCount)
{
	m_TotalDrawCalls++;

	if (!m_Shader)
	{
		m_Shader = &shader;
		m_Shader->Bind();
	}

	if (m_Shader->Id != (&shader)->Id)
	{
		Draw();

		m_Shader = &shader;
		m_Shader->Bind();
	}

	//CheckBuffer(0, 0, m_VertexCount * sizeof(Vertex), m_IndexCount * sizeof(unsigned int));

	AddIndices(indices, iCount);
	AddVertices(vertices, vCount);
}

void BatchRenderer::AddVertices(Vertex*& vertices, unsigned int count)
{
	unsigned long long vbSize = count * sizeof(Vertex);

	std::vector<Vertex> vecVertices(vertices, vertices + count);

	GLLog(glBufferSubData(GL_ARRAY_BUFFER, m_VBOffset, vbSize, vertices));

	m_VBOffset += vbSize;
	m_VertexCount += count;
}

void BatchRenderer::AddIndices(unsigned int*& indices, unsigned int count)
{
	unsigned long long ibSize = count * sizeof(unsigned int);

	for (size_t i = 0; i < count; i++)
		indices[i] += m_VertexCount;

	std::vector<unsigned int> vecIndices(indices, indices + count);

	GLLog(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_IBOffset, ibSize, indices));

	m_IBOffset += ibSize;
	m_IndexCount += count;
}

void BatchRenderer::CheckBuffer(unsigned long long vbOffset, unsigned long long ibOffset, unsigned long long vbSize, unsigned long long ibSize) const
{
	Vertex* vertices = new Vertex[MaxVertexCount];
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, MaxVertexCount * sizeof(Vertex), (Vertex*)vertices);
	unsigned int* indices = new unsigned int[MaxIndexCount];
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, MaxIndexCount * sizeof(unsigned int), (unsigned int*)indices);

	std::vector<Vertex> vecVertices(vertices, vertices + MaxVertexCount);
	std::vector<unsigned int> vecIndices(indices, indices + MaxIndexCount);

	delete[] vertices;
	delete[] indices;
}