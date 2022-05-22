#pragma once

#include "RenderData.h"
#include "../Component.h"
#include "../../Shader/Shader.h"
#include "../../../Core/GL/Texture/Texture.h"
#include "../../../Core/GL/VertexArray/VertexArray.h"

class Renderer : public Component
{
private:
	Shader* m_Shader;
	Texture* m_Texture;

	float* m_vertexCoords;
	float* m_textureCoords;
	unsigned int* m_triangleIndices;

	RenderData* m_RenderData;

public:
	Renderer(float& vertexCoords, unsigned int vcSize, unsigned int& triangleIndices, unsigned int tiSize, bool is3D);
	Renderer(float& vertexCoords, unsigned int vcSize, float& textureCoords, unsigned int tcSize, unsigned int& triangleIndices, unsigned int tiSize, bool is3D);
	~Renderer();

	void SetShader(Shader& shader);
	void SetTexture(Texture& texture);

	inline RenderData* GetRenderData() const { return m_RenderData; }

private:
	float* MergeVertexCoordsNTextureCoords(bool is3D, int vbSize, float& vertexCoords, float& textureCoords);

	void Awake(VertexArray* va) override;
	void Render(const glm::mat4 vp) override;

	bool BindTexture() const;
	bool UnBindTexture() const;

	bool BindShader() const;
	bool UnBindShader() const;
	void BindToVA(VertexArray& va) const;
};