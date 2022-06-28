#include "Mesh.h"
#include <iostream>
#include <Logger/Logger.h>
#include <Window/WindowManager.h>
#include <Component/Camera/Camera.h>
#include <GameVastu/GameVastu.h>
#include <Path.h>

using namespace core;
using namespace core::gl;

Mesh::Mesh(const std::string& path) :
	MeshPath(path),
	m_TextureCount(0),
	m_SubMeshCount(0)
{}

Mesh::~Mesh()
{
	Clear();
}

void Mesh::Awake()
{
	LoadMesh(MeshPath);
}

void Mesh::Clear()
{
	m_SubMeshCount = 0;
	m_TextureCount = 0;

	for (size_t i = 0; i < m_TextureCount; i++)
		delete m_Textures[i];

	m_Vertices.clear();
	m_Indices.clear();
	m_Textures.clear();
	m_SubMeshes.clear();
}

void Mesh::LoadMesh(const std::string& path)
{
	Clear();

	if (path.empty())
		return;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path.c_str(), ASSIMP_LOAD_FLAGS);

	if (!scene)
		Log("Unable to load Mesh from path: " << path);

	InitMesh(scene, path);
}

void Mesh::SetShader(Shader& shader)
{
	m_Shader = &shader;
}

void Mesh::InitMesh(const aiScene* scene, const std::string& path)
{
	m_SubMeshCount = scene->mNumMeshes;
	m_TextureCount = scene->mNumMaterials;
	m_SubMeshes.resize(m_SubMeshCount);
	m_Textures.resize(m_TextureCount);

	InitSubMeshData(scene);

	m_Vertices.reserve(m_VertexCount);
	m_Indices.reserve(m_IndexCount);

	InitAllSubMeshes(scene);

	m_VAO = new VertexArray();
	m_VAO->Bind();
	m_vertexVB = new VertexBuffer(m_VertexCount * sizeof(Vertex));
	m_vertexVB->Bind();
	m_indexBuffer = new IndexBuffer(&m_Indices[0], GL_UNSIGNED_INT, m_IndexCount);
	m_indexBuffer->Bind();
	m_VBLayout = new VertexBufferLayout();
	m_VBLayout->Push<Vertex>(m_vertexVB->Id, 1);
	m_VBLayout->Bind();

	InitMaterials(scene, path);
}

void Mesh::InitSubMeshData(const aiScene* scene)
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

void Mesh::InitAllSubMeshes(const aiScene* scene)
{
	for (size_t i = 0; i < m_SubMeshCount; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		InitSubMesh(mesh);
	}
}

void Mesh::InitSubMesh(const aiMesh* mesh)
{
	const aiVector3D zeroV3D(0.0f, 0.0f, 0.0f);

	// Vertices Populate
	size_t vertices = mesh->mNumVertices;
	for (size_t i = 0; i < vertices; i++)
	{
		const auto pos = mesh->mVertices[i];
		const auto normal = mesh->mNormals[i];
		const auto texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zeroV3D;

		m_Vertices.push_back({
				{ pos.x, pos.y, pos.z },
				{ 1.0f, 1.0f, 1.0f, 1.0f },
				{ texCoord.x, texCoord.y },
				1,
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

void Mesh::InitMaterials(const aiScene* scene, const std::string& path)
{
	std::string dir = GetParentDirectory(path);

	size_t materials = scene->mNumMaterials;

	for (size_t i = 0; i < materials; i++)
	{
		const aiMaterial* material = scene->mMaterials[i];

		m_Textures[i] = NULL;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString aiTexPath;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == aiReturn_SUCCESS)
			{
				std::string texPath(aiTexPath.data);

				if (texPath.substr(0, 2) == ".\\")
					texPath = texPath.substr(2, texPath.size() - 2);

				texPath = dir + "/" + texPath;

				m_Textures[i] = new Texture(texPath);

				// Need to add load error check
			}
		}
	}
}

void Mesh::Render()
{
	m_VAO->Bind();
	m_Shader->Bind();

	auto window = WindowManager::getInstance()->GetCurrentWindow();
	glm::mat4 mvp = window->GetCamera().GetProjectionMatrix() * window->GetCamera().GetViewMatrix() * gameVastu->m_transform->GetTransformMatrix();

	std::vector<Vertex> vertices = m_Vertices;
	for (size_t i = 0; i < m_VertexCount; i++)
	{
		glm::vec4 position(vertices[i].Position.x, vertices[i].Position.y, vertices[i].Position.z, 1.0f);
		position = mvp * position;
		vertices[i].Position.x = position.x;
		vertices[i].Position.y = position.y;
		vertices[i].Position.z = position.z;
		vertices[i].Position.w = position.w;
	}

	m_vertexVB->Bind();

	GLLog(glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(Vertex), &vertices[0]));

	for (size_t i = 0; i < m_SubMeshCount; i++)
	{
		unsigned int materialIndex = m_SubMeshes[i].MaterialIndex;

		assert(materialIndex < m_TextureCount);

		if (m_Textures[materialIndex])
			m_Textures[materialIndex]->BindToUnit(0);

		auto startIndex = sizeof(unsigned int) * m_SubMeshes[i].BaseIndex;	// since IndexBuffer is already populated, we just provide starting offset

		GLLog(glDrawElementsBaseVertex(GL_TRIANGLES, m_SubMeshes[i].IndexCount, GL_UNSIGNED_INT, (void*)startIndex, m_SubMeshes[i].BaseVertex));
	}

	m_Shader->UnBind();
	m_VAO->UnBind();
}

void Mesh::OnInspectorGUI()
{
	if (ImGui::CollapsingHeader("Render Info"))
	{
		ImGui::Text("Vertex Count: ");
		ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
		ImGui::Text(std::to_string(m_Vertices.size()).c_str());
		ImGui::Text("Index Count: ");
		ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
		ImGui::Text(std::to_string(m_Indices.size()).c_str());
		ImGui::Text("Triangles: ");
		ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
		ImGui::Text(std::to_string(m_TriangleCount).c_str());
	}

	if (ImGui::CollapsingHeader("Shader Info"))
	{
		ImGui::Text("Shader Id: ");
		ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
		ImGui::Text(std::to_string(m_Shader->Id).c_str());
		ImGui::Text("Shader Path: ");
		ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
		ImGui::Text(m_Shader->Path.c_str());
	}

	if (ImGui::CollapsingHeader("Texture Info"))
	{
		for (size_t i = 0; i < m_TextureCount; i++)
		{
			std::string str = std::to_string(i) + ". ";
			if (m_Textures[i])
				str = str + m_Textures[i]->FileName;
			ImGui::Text(str.c_str());
		}
	}

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

	if (ImGui::CollapsingHeader("Vertices"))
	{
		if (ImGui::BeginTable("Vertices", 4))
		{
			ImGui::TableNextColumn();
			ImGui::TableHeader("#");
			ImGui::TableNextColumn();
			ImGui::TableHeader("X");
			ImGui::TableNextColumn();
			ImGui::TableHeader("Y");
			ImGui::TableNextColumn();
			ImGui::TableHeader("Z");
			for (size_t i = 0; i < m_VertexCount; i++)
			{
				ImGui::PushID(i);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(std::to_string(i).c_str());
				ImGui::TableNextColumn();
				ImGui::DragFloat("X", &m_Vertices[i].Position.x);
				ImGui::TableNextColumn();
				ImGui::DragFloat("Y", &m_Vertices[i].Position.y);
				ImGui::TableNextColumn();
				ImGui::DragFloat("Z", &m_Vertices[i].Position.z);
				ImGui::PopID();
			}
		}
		ImGui::EndTable();
	}
}