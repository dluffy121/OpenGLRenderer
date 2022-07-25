#pragma once

#include <RenderIntent/RenderIntent.h>
#include <RenderIntent/RenderResource.h>

class StandardRenderResource : public RenderResource
{
public:
	StandardRenderResource(unsigned int index, RenderResourceConfig resourceConfig);
	StandardRenderResource(unsigned int index, const StandardRenderResource& resourceConfig);
	~StandardRenderResource();
};

class StandardRenderIntent : public RenderIntent
{
public:
	unsigned int GetRenderResource(RenderResourceConfig config = RenderResourceConfig()) override;
	void DeleteRenderResource(unsigned int index) override;

	void AddVertices(unsigned int index, unsigned int size, const void* data) override;
	void RemoveVertices(unsigned int index, unsigned int size) override;
	void AddIndices(unsigned int index, unsigned int count, unsigned int* data) override;
	void RemoveIndices(unsigned int index, unsigned int count) override;
	void PushIntToLayout(unsigned int index, unsigned int count) override;
	void PushFloatToLayout(unsigned int index, unsigned int count) override;
	void PushVertexToLayout(unsigned int index, unsigned int count) override;

	void Reset() override;
	void Draw() override;
	void Draw(unsigned int index, core::gl::Material* material, unsigned int indexCount, unsigned int startIndex, unsigned int startVertex) override;

private:
	void BindRenderResource(unsigned int index) override;
	void UnBindRenderResource(unsigned int index) override;
};