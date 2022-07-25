#pragma once

#include <RenderIntent/RenderIntent.h>
#include <RenderIntent/RenderResource.h>

const size_t MAX_TRIANGLE_COUNT = 30000;
const size_t MAX_VERTEX_COUNT = MAX_TRIANGLE_COUNT * 3;
const size_t MAX_INDEX_COUNT = MAX_TRIANGLE_COUNT * 3;

class BatchRenderResource : public RenderResource
{
public:
	unsigned int m_VBOffset;
	unsigned int m_VBSize;

	unsigned int m_IBOffset;
	unsigned int m_IBCount;

public:
	BatchRenderResource(unsigned int index, RenderResourceConfig resourceConfig);
	BatchRenderResource(unsigned int index, const BatchRenderResource& resourceConfig);
	~BatchRenderResource();
};

class BatchRenderIntent : public RenderIntent
{
private:
	unsigned int m_StartVertex = 0;
	unsigned int m_StartIndex = 0;
	unsigned int m_IndexCount = 0;
	core::gl::Material* m_CurrentMaterial = nullptr;

public:
	unsigned int GetRenderResource(RenderResourceConfig config = RenderResourceConfig()) override;
	void DeleteRenderResource(unsigned int id) override;

	void AddVertices(unsigned int id, unsigned int size, const void* data) override;
	void RemoveVertices(unsigned int id, unsigned int size) override;
	void AddIndices(unsigned int id, unsigned int count, unsigned int* data) override;
	void RemoveIndices(unsigned int id, unsigned int count) override;
	void PushIntToLayout(unsigned int id, unsigned int count) override;
	void PushFloatToLayout(unsigned int id, unsigned int count) override;
	void PushVertexToLayout(unsigned int id, unsigned int count) override;

	void Reset() override;
	void Draw() override;
	void Draw(unsigned int id, core::gl::Material* material, unsigned int indexCount, unsigned int startIndex, unsigned int startVertex) override;

private:
	void BindRenderResource(unsigned int id) override;
	void UnBindRenderResource(unsigned int id) override;
};