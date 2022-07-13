#include "MeshRenderer.h"
#include <imgui/imgui.h>
#include <Logger/Logger.h>
#include <Path.h>
#include <Window/WindowManager.h>
#include <GameVastu/GameVastu.h>

using namespace core;
using namespace core::gl;

MeshRenderer::MeshRenderer(const std::string& path, unsigned int loadFlags) :
	Renderer(),
	MeshPath(path),
	m_LoadFlags(loadFlags),
	m_SubMeshCount(0)
{}

MeshRenderer::~MeshRenderer()
{
	UnLoadResources();
}

void MeshRenderer::UnLoadResources()
{
	m_SubMeshCount = 0;
	m_SubMeshes.clear();
}

void MeshRenderer::LoadResources()
{
	if (MeshPath.empty())
		return;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(MeshPath.c_str(), m_LoadFlags);

	if (!scene)
		Log("Unable to load Mesh from path: " << MeshPath);

	InitMesh(scene, MeshPath);
}

void MeshRenderer::InitMesh(const aiScene* scene, const std::string& path)
{
	m_SubMeshCount = scene->mNumMeshes;
	m_MaterialCount = scene->mNumMaterials;
	m_SubMeshes.resize(m_SubMeshCount);
	m_Materials.resize(m_MaterialCount);

	InitSubMeshData(scene);

	m_Vertices.reserve(m_VertexCount);
	m_Indices.reserve(m_IndexCount);

	InitAllSubMeshes(scene);

	m_VAO = new VertexArray();
	m_VAO->Bind();
	m_VertexBuffer = new VertexBuffer(m_VertexCount * sizeof(Vertex));
	m_VertexBuffer->Bind();
	m_IndexBuffer = new IndexBuffer(&m_Indices[0], GL_UNSIGNED_INT, m_IndexCount);
	m_IndexBuffer->Bind();
	m_VBLayout = new VertexBufferLayout();
	m_VBLayout->Push<Vertex>(m_VertexBuffer->Id, 1);
	m_VBLayout->Bind();

	InitMaterials(scene, path);
}

void MeshRenderer::InitSubMeshData(const aiScene* scene)
{
	for (size_t i = 0; i < m_SubMeshCount; i++)
	{
		m_SubMeshes[i].MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
		m_SubMeshes[i].IndexCount = scene->mMeshes[i]->mNumFaces * 3;	// Since meshes were triangulated
		m_SubMeshes[i].BaseVertex = m_VertexCount;
		m_SubMeshes[i].BaseIndex = m_IndexCount;

		m_VertexCount += scene->mMeshes[i]->mNumVertices;
		m_IndexCount += m_SubMeshes[i].IndexCount;
		m_TriangleCount += scene->mMeshes[i]->mNumFaces;
	}
}

void MeshRenderer::InitAllSubMeshes(const aiScene* scene)
{
	for (size_t i = 0; i < m_SubMeshCount; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		InitSubMesh(mesh);
	}
}

void MeshRenderer::InitSubMesh(const aiMesh* mesh)
{
	const aiVector3D zeroV3D(0.0f, 0.0f, 0.0f);

	// Vertices Populate
	unsigned int vertices = mesh->mNumVertices;
	for (unsigned int i = 0; i < vertices; i++)
	{
		const auto pos = mesh->mVertices[i];
		const auto normal = mesh->mNormals ? mesh->mNormals[i] : aiVector3D(0.0f, 1.0f, 0.0f);
		const auto texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zeroV3D;

		m_Vertices.push_back({
				{ pos.x, pos.y, pos.z },
				{ 1.0f, 1.0f, 1.0f, 1.0f },
				{ },
				{ texCoord.x, texCoord.y },
				0,
				{ normal.x, normal.y, normal.z }
			});
	}

	// Indices Populate
	size_t faces = mesh->mNumFaces;
	for (size_t i = 0, j = 0;
		i < faces; i++)
	{
		const auto face = mesh->mFaces[i];

		assert(face.mNumIndices == 3);

		m_Indices.push_back(face.mIndices[0]);
		m_Indices.push_back(face.mIndices[1]);
		m_Indices.push_back(face.mIndices[2]);
	}
}

void MeshRenderer::InitMaterials(const aiScene* scene, const std::string& path)
{
	std::string dir = GetParentDirectory(path);

	size_t materials = scene->mNumMaterials;

	for (size_t i = 0; i < materials; i++)
	{
		const aiMaterial* aiMaterial = scene->mMaterials[i];

		m_Materials[i] = new Material();
		m_Materials[i]->m_Shader = m_Shader;

		LoadDiffuseTexture(dir, aiMaterial, m_Materials[i]);

		LoadSpecularTexture(dir, aiMaterial, m_Materials[i]);

		LoadColors(aiMaterial, m_Materials[i]);
	}
}

void MeshRenderer::LoadDiffuseTexture(const std::string& dir, const aiMaterial* aiMaterial, core::gl::Material*& material)
{
	material->m_DiffuseTexture = NULL;

	if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString aiTexPath;

		if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == aiReturn_SUCCESS)
		{
			std::string texPath(aiTexPath.data);

			if (texPath.substr(0, 2) == ".\\")
				texPath = texPath.substr(2, texPath.size() - 2);

			texPath = dir + "/" + texPath;

			material->m_DiffuseTexture = new Texture(texPath);

			// Need to add load error check
		}
	}
}

void MeshRenderer::LoadSpecularTexture(const std::string& dir, const aiMaterial* aiMaterial, core::gl::Material*& material)
{
	material->m_SpecularExponent = NULL;

	if (aiMaterial->GetTextureCount(aiTextureType_SHININESS) > 0)
	{
		aiString aiTexPath;

		if (aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiTexPath) == aiReturn_SUCCESS)
		{
			std::string texPath(aiTexPath.data);

			if (texPath.substr(0, 2) == ".\\")
				texPath = texPath.substr(2, texPath.size() - 2);

			texPath = dir + "/" + texPath;

			material->m_SpecularExponent = new Texture(texPath);

			// Need to add load error check
		}
	}
}

void MeshRenderer::LoadColors(const aiMaterial* aiMaterial, core::gl::Material*& material)
{
	aiColor3D aiAmbientColor;

	if (aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbientColor) == aiReturn_SUCCESS)
	{
		material->m_AmbientColor = { aiAmbientColor.r, aiAmbientColor.g, aiAmbientColor.b };
	}

	aiColor3D aiDiffuseColor;

	if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuseColor) == aiReturn_SUCCESS)
	{
		material->m_DiffuseColor = { aiDiffuseColor.r, aiDiffuseColor.g, aiDiffuseColor.b };
	}

	aiColor3D aiSpecularColor;

	if (aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecularColor) == aiReturn_SUCCESS)
	{
		material->m_SpecularColor = { aiSpecularColor.r, aiSpecularColor.g, aiSpecularColor.b };
	}
}

void MeshRenderer::Draw()
{
	for (size_t i = 0; i < m_SubMeshCount; i++)
	{
		unsigned int materialIndex = m_SubMeshes[i].MaterialIndex;

		assert(materialIndex < m_MaterialCount);

		m_Materials[materialIndex]->Bind();

		auto startIndex = sizeof(unsigned int) * m_SubMeshes[i].BaseIndex;	// since IndexBuffer is already populated, we just provide starting offset

		GLLog(glDrawElementsBaseVertex(GL_TRIANGLES, m_SubMeshes[i].IndexCount, GL_UNSIGNED_INT, (void*)startIndex, m_SubMeshes[i].BaseVertex));
	}
}

void MeshRenderer::OnInspectorGUI()
{
	Renderer::OnInspectorGUI();

	if (ImGui::CollapsingHeader("Sub-Meshes Info"))
	{
		ImGui::Indent();
		for (size_t i = 0; i < m_SubMeshCount; i++)
		{
			std::string header = "SubMesh " + std::to_string(i);
			if (ImGui::CollapsingHeader(header.c_str()))
			{
				std::string str = "BaseIndex: " + std::to_string(m_SubMeshes[i].BaseIndex);
				ImGui::Text(str.c_str());
				str = "BaseVertex: " + std::to_string(m_SubMeshes[i].BaseVertex);
				ImGui::Text(str.c_str());
				str = "IndexCount: " + std::to_string(m_SubMeshes[i].IndexCount);
				ImGui::Text(str.c_str());
				str = "MaterialIndex: " + std::to_string(m_SubMeshes[i].MaterialIndex);
				ImGui::Text(str.c_str());
			}
		}
		ImGui::Unindent();
	}
}