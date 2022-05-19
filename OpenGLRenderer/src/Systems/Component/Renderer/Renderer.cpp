#include "Renderer.h"

Renderer::Renderer(float& vertexCoords, unsigned int vcSize, unsigned int& triangleIndices, unsigned int tiSize, bool is3D)
{
	m_vertexCoords = &vertexCoords;
	m_triangleIndices = &triangleIndices;

	unsigned int dimensions = is3D ? 3 : 2;

	VertexBuffer* vb = new VertexBuffer(m_vertexCoords, vcSize * sizeof(float));
	IndexBuffer* ib = new IndexBuffer(m_triangleIndices, GL_UNSIGNED_INT, tiSize);
	VertexBufferLayout* layout = new VertexBufferLayout();
	layout->Push<float>(2);			// for Vertex coords

	m_RenderData = new RenderData(*vb, *layout, *ib);
}

Renderer::Renderer(float& vertexCoords, unsigned int vcSize, float& textureCoords, unsigned int tcSize, unsigned int& triangleIndices, unsigned int tiSize, bool is3D)
{
	m_vertexCoords = &vertexCoords;
	m_textureCoords = &textureCoords;
	m_triangleIndices = &triangleIndices;

	unsigned int dimensions = is3D ? 3 : 2;

	int vbSize = vcSize + tcSize;

	float* vertexData = MergeVertexCoordsNTextureCoords(is3D, vbSize, vertexCoords, textureCoords);

	VertexBuffer* vb = new VertexBuffer(vertexData, vbSize * sizeof(float));
	IndexBuffer* ib = new IndexBuffer(m_triangleIndices, GL_UNSIGNED_INT, tiSize);
	VertexBufferLayout* layout = new VertexBufferLayout();
	layout->Push<float>(2);			// for Vertex coords
	layout->Push<float>(2);			// for Texture coords

	m_RenderData = new RenderData(*vb, *layout, *ib);
}

Renderer::~Renderer()
{
	delete m_RenderData;
}

void Renderer::SetShader(Shader& shader)
{
	m_Shader = &shader;
}

void Renderer::SetTexture(Texture& texture)
{
	m_Texture = &texture;
}

void Renderer::BindToVA(VertexArray& va) const
{
	va.AddBuffer(*m_RenderData->m_vb, *m_RenderData->m_layout);
}

bool Renderer::BindShader() const
{
	if (m_Shader == nullptr)
		return false;

	m_Shader->Bind();
	return true;
}

bool Renderer::UnBindShader() const
{
	if (m_Shader == nullptr)
		return false;

	m_Shader->UnBind();
	return true;
}

bool Renderer::UpdateShaderMVP(glm::mat4 mvp) const
{
	if (m_Shader == nullptr)
		return false;

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
	m_Shader->UnBind();
	return true;
}

bool Renderer::BindTexture() const
{
	if (m_Texture == NULL)
		return false;

	m_Texture->Bind();
	return true;
}

bool Renderer::UnBindTexture() const
{
	if (m_Texture == NULL)
		return false;

	m_Texture->UnBind();
	return true;
}

float* Renderer::MergeVertexCoordsNTextureCoords(bool is3D, int vbSize, float& vertexCoords, float& textureCoords)
{
	float* vertexData = new float[vbSize];
	unsigned int divisor = is3D ? 6 : 4;
	unsigned int remainder = is3D ? 3 : 2;
	int x = 0;
	int y = 0;
	for (int i = 0; i < vbSize; i++)
	{
		if (i % divisor < remainder)
		{
			vertexData[i] = (&vertexCoords)[x++];
			continue;
		}

		vertexData[i] = (&textureCoords)[y++];
	}

	return vertexData;
}