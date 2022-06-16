#pragma once

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ASSIMP_LOAD_FLAGS ( aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices )

class Mesh
{
private:
	unsigned int m_subMeshCount;
	unsigned int m_textureCount;

public:
	Mesh();
	~Mesh();

	void LoadMesh(const std::string& path);

private:
	void GenerateMeshData(const aiScene* scene, const std::string& path);
};