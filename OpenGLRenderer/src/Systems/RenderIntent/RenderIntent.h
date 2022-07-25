#pragma once

#include <GL/VertexArray/VertexArray.h>
#include <GL/VertexBuffer/VertexBuffer.h>
#include <GL/IndexBuffer/IndexBuffer.h>
#include <GL/VertexBufferLayout/VertexBufferLayout.h>
#include <GL/Material/Material.h>
#include <RenderIntent/RenderResource.h>
#include <map>

struct RenderResourceConfig
{
	bool DynamicVB = false;
	unsigned int VBSize = 0;

	bool DynamicIB = false;
	unsigned int IBCount = 0;
	unsigned int IBType = GL_UNSIGNED_INT;
};

class RenderIntent
{
public:
	bool Enable;
	unsigned int DrawCalls;

protected:
	unsigned int NextIndex;

protected:
	RenderResourceConfig m_RenderResourceConfig;
	std::map<unsigned int, RenderResource*> m_RenderResources;

public:
	virtual unsigned int GetRenderResource(RenderResourceConfig config = RenderResourceConfig()) = 0;
	virtual void DeleteRenderResource(unsigned int id) = 0;

	virtual void AddVertices(unsigned int id, unsigned int size, const void* data) = 0;
	virtual void RemoveVertices(unsigned int id, unsigned int size) = 0;
	virtual void AddIndices(unsigned int id, unsigned int count, unsigned int* data) = 0;
	virtual void RemoveIndices(unsigned int id, unsigned int count) = 0;
	virtual void PushIntToLayout(unsigned int id, unsigned int count) = 0;
	virtual void PushFloatToLayout(unsigned int id, unsigned int count) = 0;
	virtual void PushVertexToLayout(unsigned int id, unsigned int count) = 0;

	virtual void Reset() = 0;
	virtual void Draw() = 0;
	virtual void Draw(unsigned int id, core::gl::Material* material, unsigned int indexCount, unsigned int startIndex, unsigned int startVertex) = 0;

private:
	virtual void BindRenderResource(unsigned int id) = 0;
	virtual void UnBindRenderResource(unsigned int id) = 0;
};

static void CheckBuffers(unsigned int vbsize, unsigned int ibcount)
{
	std::vector<core::Vertex> vertices(vbsize / sizeof(core::Vertex));
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, vbsize, &vertices[0]);
	std::vector<unsigned int> indices(ibcount);
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * ibcount, &indices[0]);
}