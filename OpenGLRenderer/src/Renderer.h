#pragma once

#include "Shader.h"
#include "Component.h"
#include "RenderData.h"
#include "glm/glm.hpp"

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

	void BindToVA(VertexArray& va) const;

	inline RenderData* GetRenderData() const { return m_RenderData; }

	bool BindShader() const;
	bool UnBindShader() const;
	bool UpdateShaderMVP(glm::mat4 mvp) const;

	bool BindTexture() const;
	bool UnBindTexture() const;

private:
	float* MergeVertexCoordsNTextureCoords(bool is3D, int vbSize, float& vertexCoords, float& textureCoords);
};