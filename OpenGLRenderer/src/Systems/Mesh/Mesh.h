#pragma once

#include <Component/Component.h>
#include <string>
#include <vector>
#include <GL/VertexArray/VertexArray.h>
#include <GL/VertexBuffer/VertexBuffer.h>
#include <GL/IndexBuffer/IndexBuffer.h>
#include <GL/VertexBufferLayout/VertexBufferLayout.h>
#include <GL/Texture/Texture.h>
#include <Shader/Shader.h>
#include <Math/Math.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ASSIMP_LOAD_FLAGS ( aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices )

struct SubMeshData;

class Mesh : public Component
{
public:
	std::string MeshPath;

private:
	core::gl::VertexArray* m_VAO;
	core::gl::VertexBuffer* m_vertexVB;
	core::gl::IndexBuffer* m_indexBuffer;
	core::gl::VertexBufferLayout* m_VBLayout;

	unsigned int m_VertexCount;
	std::vector<core::Vertex> m_Vertices;
	unsigned int m_IndexCount;
	std::vector<unsigned int> m_Indices;

	unsigned int m_TextureCount;
	std::vector<core::gl::Texture*> m_Textures;

	unsigned int m_SubMeshCount;
	std::vector<SubMeshData> m_SubMeshes;

	Shader* m_Shader;

	unsigned int m_TriangleCount;

public:
	Mesh(const std::string& path = "");
	~Mesh();

	void Clear();
	void LoadMesh(const std::string& path = "");

	void SetShader(Shader& shader);

private:
	void InitMesh(const aiScene* scene, const std::string& path);
	void InitSubMeshData(const aiScene* scene);
	void InitAllSubMeshes(const aiScene* scene);
	void InitSubMesh(const aiMesh* mesh);
	void InitMaterials(const aiScene* scene, const std::string& path);

	void Awake() override;
	void Render() override;
	void OnInspectorGUI() override;
};

struct SubMeshData
{
	unsigned int MaterialIndex = -1;
	unsigned int IndexCount = 0;
	unsigned int BaseVertex = 0;
	unsigned int BaseIndex = 0;
};