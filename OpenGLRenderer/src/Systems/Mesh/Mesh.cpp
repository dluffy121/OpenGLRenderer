#include "Mesh.h"
#include <iostream>
#include <Logger/Logger.h>
#include <Window/WindowManager.h>
#include <Camera/Camera.h>
#include <GameVastu/GameVastu.h>
#include <Path.h>

using namespace core;
using namespace core::gl;

Mesh::Mesh(const std::string& path, unsigned int loadFlags) :
	MeshPath(path),
	m_LoadFlags(loadFlags),
	//m_TextureCount(0),
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
	m_MaterialCount = 0;

	delete m_VertexBuffer;
	delete m_IndexBuffer;
	delete m_VBLayout;
	delete m_VAO;

	for (size_t i = 0; i < m_MaterialCount; i++)
		delete m_Materials[i];

	m_Vertices.clear();
	m_Indices.clear();
	m_Materials.clear();
	m_SubMeshes.clear();
}

void Mesh::LoadMesh(const std::string& path)
{
	Clear();

	if (path.empty())
		return;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path.c_str(), m_LoadFlags);

	if (!scene)
		Log("Unable to load Mesh from path: " << path);

	InitMesh(scene, path);
}

void Mesh::SetShader(Shader& shader)
{
	m_Shader = &shader;
	m_Shader->Bind();
	m_Shader->SetUniform1i("u_DiffuseTexture", 0);
	m_Shader->SetUniform1i("u_SpecularExponent", 1);
	m_Shader->UnBind();
}

void Mesh::InitMesh(const aiScene* scene, const std::string& path)
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

void Mesh::InitMaterials(const aiScene* scene, const std::string& path)
{
	std::string dir = GetParentDirectory(path);

	size_t materials = scene->mNumMaterials;

	for (size_t i = 0; i < materials; i++)
	{
		const aiMaterial* aiMaterial = scene->mMaterials[i];

		m_Materials[i] = new Material();

		LoadDiffuseTexture(dir, aiMaterial, i);

		LoadSpecularTexture(dir, aiMaterial, i);

		LoadColors(aiMaterial, i);
	}
}

void Mesh::LoadDiffuseTexture(const std::string& dir, const aiMaterial* aiMaterial, unsigned int index)
{
	m_Materials[index]->m_DiffuseTexture = NULL;

	if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString aiTexPath;

		if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == aiReturn_SUCCESS)
		{
			std::string texPath(aiTexPath.data);

			if (texPath.substr(0, 2) == ".\\")
				texPath = texPath.substr(2, texPath.size() - 2);

			texPath = dir + "/" + texPath;

			m_Materials[index]->m_DiffuseTexture = new Texture(texPath);

			// Need to add load error check
		}
	}
}

void Mesh::LoadSpecularTexture(const std::string& dir, const aiMaterial* aiMaterial, unsigned int index)
{
	m_Materials[index]->m_SpecularExponent = NULL;

	if (aiMaterial->GetTextureCount(aiTextureType_SHININESS) > 0)
	{
		aiString aiTexPath;

		if (aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiTexPath) == aiReturn_SUCCESS)
		{
			std::string texPath(aiTexPath.data);

			if (texPath.substr(0, 2) == ".\\")
				texPath = texPath.substr(2, texPath.size() - 2);

			texPath = dir + "/" + texPath;

			m_Materials[index]->m_SpecularExponent = new Texture(texPath);

			// Need to add load error check
		}
	}
}

void Mesh::LoadColors(const aiMaterial* aiMaterial, unsigned int index)
{
	aiColor3D aiAmbientColor;

	if (aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbientColor) == aiReturn_SUCCESS)
	{
		m_Materials[index]->m_AmbientColor = { aiAmbientColor.r, aiAmbientColor.g, aiAmbientColor.b };
	}

	aiColor3D aiDiffuseColor;

	if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuseColor) == aiReturn_SUCCESS)
	{
		m_Materials[index]->m_DiffuseColor = { aiDiffuseColor.r, aiDiffuseColor.g, aiDiffuseColor.b };
	}

	aiColor3D aiSpecularColor;

	if (aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiDiffuseColor) == aiReturn_SUCCESS)
	{
		m_Materials[index]->m_SpecularColor = { aiSpecularColor.r, aiSpecularColor.g, aiSpecularColor.b };
	}
}

void Mesh::Render()
{
	m_VAO->Bind();
	m_Shader->Bind();

	auto window = WindowManager::getInstance()->GetCurrentWindow();
	auto camera = window->GetCameraManager().GetCamera();
	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * gameVastu->m_transform->GetTransformMatrix();

	window->GetLightingManager().UpdateShaderLightData(*m_Shader, *camera->GetGameVastu()->m_transform, *gameVastu->m_transform);

	std::vector<Vertex> vertices = m_Vertices;
	for (size_t i = 0; i < m_VertexCount; i++)
	{
		glm::vec4 pixelPosition(vertices[i].Position.x, vertices[i].Position.y, vertices[i].Position.z, 1.0f);
		pixelPosition = mvp * pixelPosition;
		vertices[i].PixelPosition.x = pixelPosition.x;
		vertices[i].PixelPosition.y = pixelPosition.y;
		vertices[i].PixelPosition.z = pixelPosition.z;
		vertices[i].PixelPosition.w = pixelPosition.w;
	}

	m_VertexBuffer->Bind();
	GLLog(glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(Vertex), &vertices[0]));

	for (size_t i = 0; i < m_SubMeshCount; i++)
	{
		unsigned int materialIndex = m_SubMeshes[i].MaterialIndex;

		assert(materialIndex < m_MaterialCount);

		if (m_Materials[materialIndex]->m_DiffuseTexture)
			m_Materials[materialIndex]->m_DiffuseTexture->BindToUnit(0);
		if (m_Materials[materialIndex]->m_SpecularExponent)
			m_Materials[materialIndex]->m_SpecularExponent->BindToUnit(1);

		auto ambientColor = m_Materials[materialIndex]->m_AmbientColor;
		m_Shader->SetUniform3f("u_Material.ambientColor", ambientColor.x, ambientColor.y, ambientColor.z);
		auto diffuseColor = m_Materials[materialIndex]->m_DiffuseColor;
		m_Shader->SetUniform3f("u_Material.diffuseColor", diffuseColor.x, diffuseColor.y, diffuseColor.z);
		auto specularColor = m_Materials[materialIndex]->m_SpecularColor;
		m_Shader->SetUniform3f("u_Material.specularColor", specularColor.x, specularColor.y, specularColor.z);

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

	if (ImGui::CollapsingHeader("Material Info"))
	{
		ImGui::Indent();
		for (size_t i = 0; i < m_MaterialCount; i++)
		{
			std::string header = "Material " + std::to_string(i);
			if (ImGui::CollapsingHeader(header.c_str()))
			{
				ImGui::PushID(i);

				Material*& matrial = m_Materials[i];

				ImGui::ColorEdit3("Ambient Color", matrial->m_AmbientColor);
				ImGui::ColorEdit3("Diffuse Color", matrial->m_DiffuseColor);
				ImGui::ColorEdit3("Specular Color", matrial->m_SpecularColor);

				if (m_Materials[i]->m_DiffuseTexture)
				{
					ImGui::Text("Diffuse Texture: ");
					ImGui::Indent();
					ImGui::Text(matrial->m_DiffuseTexture->FileName.c_str());
					ImGui::Text(matrial->m_DiffuseTexture->FilePath.c_str());
					ImGui::Unindent();
				}

				if (m_Materials[i]->m_SpecularExponent)
				{
					ImGui::Text("Specular Exponent Texture: ");
					ImGui::Indent();
					ImGui::Text(matrial->m_SpecularExponent->FileName.c_str());
					ImGui::Text(matrial->m_SpecularExponent->FilePath.c_str());
					ImGui::Unindent();
				}

				ImGui::PopID();
			}
		}
		ImGui::Unindent();
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
		if (ImGui::BeginTable("Vertices", 3))
		{
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

	if (ImGui::CollapsingHeader("Colors"))
	{
		for (size_t i = 0; i < m_VertexCount; i++)
		{
			float color[4] = { m_Vertices[i].Color.x, m_Vertices[i].Color.y, m_Vertices[i].Color.z, m_Vertices[i].Color.w };
			if (ImGui::ColorEdit4(std::to_string(i).c_str(), color))
			{
				m_Vertices[i].Color.x = color[0];
				m_Vertices[i].Color.y = color[1];
				m_Vertices[i].Color.z = color[2];
				m_Vertices[i].Color.w = color[3];
			}
		}
	}

	if (ImGui::CollapsingHeader("Texture Coords"))
	{
		if (ImGui::BeginTable("Texture Coords", 2))
		{
			ImGui::TableNextColumn();
			ImGui::TableHeader("X");
			ImGui::TableNextColumn();
			ImGui::TableHeader("Y");
			for (size_t i = 0; i < m_VertexCount; i++)
			{
				ImGui::PushID(i);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::DragFloat("X", &m_Vertices[i].TexCoords.x, 0.01f, 0.0f, 1.0f);
				ImGui::TableNextColumn();
				ImGui::DragFloat("Y", &m_Vertices[i].TexCoords.y, 0.01f, 0.0f, 1.0f);
				ImGui::PopID();
			}
		}
		ImGui::EndTable();
	}

	if (ImGui::CollapsingHeader("Texture Index"))
	{
		for (size_t i = 0; i < m_VertexCount; i++)
		{
			ImGui::DragInt(std::to_string(i).c_str(), &m_Vertices[i].TexID);
		}
	}
}