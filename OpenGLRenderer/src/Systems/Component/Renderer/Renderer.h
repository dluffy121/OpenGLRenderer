#pragma once

#include "RenderData.h"
#include "../../../Core/GL/Texture/Texture.h"
#include "../../../Core/GL/VertexArray/VertexArray.h"
#include <Component/Component.h>
#include <Shader/Shader.h>
#include "../../../Core/Math.h"

using namespace core;
using namespace core::gl;

class Renderer : public Component
{
private:
	Shader* m_Shader;
	std::unordered_map<int, Texture*> m_Textures;

	Vertex* m_Vertices;
	Vertex* _Vertices;
	const unsigned int m_VertexCount;

	unsigned int* m_Indices;
	unsigned int* _Indices;
	unsigned int m_IndexCount;

	int m_triangleCount;

public:
	Renderer(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);
	~Renderer();

	void SetShader(Shader& shader);
	void AddTexture(int samplerId, Texture& texture);
	void RemoveTexture(int samplerId);

private:
	void Render() override;
	void OnInspectorGUI() override;

	void BindTextures() const;
	void UnBindTexture() const;

	bool BindShader() const;
	bool UnBindShader() const;
};