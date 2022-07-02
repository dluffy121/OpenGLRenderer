#pragma once

#include <Component/Component.h>
#include <string>
#include <vector>
#include <GL/VertexArray/VertexArray.h>
#include <GL/VertexBuffer/VertexBuffer.h>
#include <GL/IndexBuffer/IndexBuffer.h>
#include <GL/VertexBufferLayout/VertexBufferLayout.h>
#include <GL/Texture/Texture.h>
#include <GL/Material/Material.h>
#include <Shader/Shader.h>
#include <Math/Math.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ASSIMP_LOAD_FLAGS_FLIP_UV ( aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices )
#define ASSIMP_LOAD_FLAGS ( aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices )

struct SubMeshData;

class Mesh : public Component
{
public:
	std::string MeshPath;

private:
	unsigned int m_LoadFlags;

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

	unsigned int m_SubMeshCount;
	std::vector<SubMeshData> m_SubMeshes;

	Shader* m_Shader;

	unsigned int m_TriangleCount;

public:
	Mesh(const std::string& path = "", unsigned int loadFlags = ASSIMP_LOAD_FLAGS);
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
	void LoadDiffuseTexture(const std::string& dir, const aiMaterial* aiMaterial, unsigned int index);
	void LoadSpecularTexture(const std::string& dir, const aiMaterial* aiMaterial, unsigned int index);
	void LoadColors(const aiMaterial* aiMaterial, unsigned int index);

	void Awake() override;
	void Render() override;
	void OnInspectorGUI() override;
};

struct SubMeshData
{
	unsigned int IndexCount = 0;
	unsigned int BaseVertex = 0;
	unsigned int BaseIndex = 0;
	unsigned int MaterialIndex = NULL;
};