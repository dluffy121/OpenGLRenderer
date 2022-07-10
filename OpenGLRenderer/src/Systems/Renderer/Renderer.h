#pragma once

#include <Component/Component.h>
#include <GL/VertexArray/VertexArray.h>
#include <GL/VertexBuffer/VertexBuffer.h>
#include <GL/IndexBuffer/IndexBuffer.h>
#include <GL/VertexBufferLayout/VertexBufferLayout.h>
#include <GL/Material/Material.h>
#include <Shader/Shader.h>
#include <Math/Math.h>

class Renderer : public Component
{
protected:
	core::gl::VertexArray* m_VAO;
	core::gl::VertexBuffer* m_VertexBuffer;
	core::gl::IndexBuffer* m_IndexBuffer;
	core::gl::VertexBufferLayout* m_VBLayout;

	unsigned int m_VertexCount;
	std::vector<core::Vertex> m_Vertices;

	unsigned int m_IndexCount;
	std::vector<unsigned int> m_Indices;

	unsigned int m_MaterialCount;
	std::vector<core::gl::Material*> m_Materials;

	Shader* m_Shader;

	unsigned int m_TriangleCount;

public:
	Renderer();
	Renderer(core::Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);
	~Renderer();

	void SetShader(Shader& shader);
	void SetVertices(core::Vertex* vertices, unsigned int count);
	void SetIndices(unsigned int* indices, unsigned int count);
	void AddMaterial(core::gl::Material*& material);
	void RemoveMaterial(core::gl::Material*& material);

protected:
	virtual void LoadResources();
	virtual void UnLoadResources();
	virtual void BindResources();
	virtual void UnBindResources();
	virtual void Prepare();
	virtual void Draw();

	virtual void OnInspectorGUI() override;

private:
	void Awake() override;
	void Render() override;
};