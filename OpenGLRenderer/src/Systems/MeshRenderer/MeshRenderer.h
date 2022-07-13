#pragma once

#include <Renderer/Renderer.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ASSIMP_LOAD_FLAGS_FLIP_UV ( aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices )
#define ASSIMP_LOAD_FLAGS ( aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices )

struct SubMeshData;

class MeshRenderer : public Renderer
{
public:
	std::string MeshPath;

protected:
	unsigned int m_LoadFlags;

	unsigned int m_SubMeshCount;
	std::vector<SubMeshData> m_SubMeshes;

public:
	MeshRenderer(const std::string& path, unsigned int loadFlags = ASSIMP_LOAD_FLAGS);
	~MeshRenderer();

protected:
	virtual void LoadResources() override;
	virtual void UnLoadResources() override;
	//virtual void Prepare() override;
	virtual void Draw() override;

	virtual void OnInspectorGUI() override;

private:
	void InitMesh(const aiScene* scene, const std::string& path);
	void InitSubMeshData(const aiScene* scene);
	void InitAllSubMeshes(const aiScene* scene);
	void InitSubMesh(const aiMesh* mesh);
	void InitMaterials(const aiScene* scene, const std::string& path);
	void LoadDiffuseTexture(const std::string& dir, const aiMaterial* aiMaterial, core::gl::Material*& material);
	void LoadSpecularTexture(const std::string& dir, const aiMaterial* aiMaterial, core::gl::Material*& material);
	void LoadColors(const aiMaterial* aiMaterial, core::gl::Material*& material);
};

struct SubMeshData
{
	unsigned int IndexCount = 0;
	unsigned int BaseVertex = 0;
	unsigned int BaseIndex = 0;
	unsigned int MaterialIndex = NULL;
};