#pragma once

#include <GL/Texture/Texture.h>
#include <GL/VertexArray/VertexArray.h>
#include <Component/Component.h>
#include <Shader/Shader.h>
#include <Math/Math.h>

class Renderer : public Component
{
private:
	Shader* m_Shader;
	std::unordered_map<int, core::gl::Texture*> m_Textures;

	core::Vertex* m_Vertices;
	core::Vertex* _Vertices;
	const unsigned int m_VertexCount;

	unsigned int* m_Indices;
	unsigned int* _Indices;
	unsigned int m_IndexCount;

	int m_triangleCount;

public:
	Renderer(core::Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);
	~Renderer();

	void SetShader(Shader& shader);
	void AddTexture(int samplerId, core::gl::Texture& texture);
	void RemoveTexture(int samplerId);

private:
	void Render() override;
	void OnInspectorGUI() override;

	void BindTextures() const;
	void UnBindTexture() const;

	bool BindShader() const;
	bool UnBindShader() const;
};