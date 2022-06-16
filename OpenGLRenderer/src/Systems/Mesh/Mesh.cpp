#include "Mesh.h"
#include <iostream>
#include "../../Core/Logger.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::LoadMesh(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path.c_str(), ASSIMP_LOAD_FLAGS);

	if (!scene)
		Log("Unable to load Mesh from path: " << path);

	GenerateMeshData(scene, path);
}

void Mesh::GenerateMeshData(const aiScene* scene, const std::string& path)
{
	m_subMeshCount = scene->mNumMeshes;
	m_textureCount= scene->mNumTextures;
}
