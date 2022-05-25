#pragma once

#include "RenderData.h"
#include "../../../Core/GL/Texture/Texture.h"
#include "../../../Core/GL/VertexArray/VertexArray.h"
#include <Component/Component.h>
#include <Shader/Shader.h>

using namespace core::gl;

class Renderer : public Component
{
private:
	Shader* m_Shader;
	Texture* m_Texture;

	float* m_vertexCoords;
	int m_vertexCoordsSize;
	float* m_textureCoords;
	int m_textureCoordsSize;
	unsigned int* m_triangleIndices;
	int m_triangleIndicesSize;
	int m_triangleCount;

	RenderData* m_RenderData;

public:
	Renderer(float& vertexCoords, unsigned int vcSize, unsigned int& triangleIndices, unsigned int tiSize, bool is3D);
	Renderer(float& vertexCoords, unsigned int vcSize, float& textureCoords, unsigned int tcSize, unsigned int& triangleIndices, unsigned int tiSize, bool is3D);
	~Renderer();

	void SetShader(Shader& shader);
	void SetTexture(Texture& texture);

	inline const RenderData& GetRenderData() const { return *m_RenderData; }

	void OnInspectorGUI() override;

private:
	float* MergeVertexCoordsNTextureCoords(bool is3D, int vbSize, float& vertexCoords, float& textureCoords);

	void Render(const glm::mat4 vp) override;

	bool BindTexture() const;
	bool UnBindTexture() const;

	bool BindShader() const;
	bool UnBindShader() const;
};