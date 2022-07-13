#include "Renderer.h"
#include <GameVastu/GameVastu.h>
#include <Logger/Logger.h>
#include <Camera/Camera.h>

using namespace core;
using namespace core::gl;

Renderer::Renderer() :
	m_VAO(NULL),
	m_VertexBuffer(NULL),
	m_IndexBuffer(NULL),
	m_VBLayout(NULL),
	m_VertexCount(0),
	m_IndexCount(0),
	m_MaterialCount(0),
	m_Shader(NULL),
	m_TriangleCount(0)
{}

Renderer::Renderer(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount, bool calculateNormals) :
	m_VAO(NULL),
	m_VertexBuffer(NULL),
	m_IndexBuffer(NULL),
	m_VBLayout(NULL),
	m_Vertices(std::vector<Vertex>(vertices, vertices + vertexCount)),
	m_VertexCount(vertexCount),
	m_Indices(std::vector<unsigned int>(indices, indices + indexCount)),
	m_IndexCount(indexCount),
	m_MaterialCount(0),
	m_Shader(NULL),
	m_TriangleCount(indexCount / 3)
{
	if (calculateNormals) CalculateNormals();
}

Renderer::~Renderer()
{
	UnLoadResources();
}

void Renderer::CalculateNormals()
{
	for (auto vertex : m_Vertices)
		vertex.Normal = Vec3(0.0f);

	for (size_t i = 0; i < m_IndexCount; i += 3)
	{
		auto v1 = m_Vertices[m_Indices[i]];
		auto v2 = m_Vertices[m_Indices[i + 1]];
		auto v3 = m_Vertices[m_Indices[i + 2]];

		auto side_1 = v2.Position - v1.Position;
		auto side_2 = v3.Position - v1.Position;

		auto normal = Vec3::Cross(side_1, side_2);

		m_Vertices[m_Indices[i]].Normal		+= normal;
		m_Vertices[m_Indices[i + 1]].Normal += normal;
		m_Vertices[m_Indices[i + 2]].Normal += normal;
	}

	for (auto vertex : m_Vertices)
		vertex.Normal.Normalize();
}

void Renderer::Awake()
{
	LoadResources();
}

void Renderer::LoadResources()
{
	m_VAO = new VertexArray();
	m_VAO->Bind();
	m_VertexBuffer = new VertexBuffer(m_VertexCount * sizeof(Vertex));
	m_VertexBuffer->Bind();
	if (!m_Indices.empty())
	{
		m_IndexBuffer = new IndexBuffer(&m_Indices[0], GL_UNSIGNED_INT, m_IndexCount);
		m_IndexBuffer->Bind();
	}
	m_VBLayout = new VertexBufferLayout();
	m_VBLayout->Push<Vertex>(m_VertexBuffer->Id, 1);
	m_VBLayout->Bind();
}

void Renderer::UnLoadResources()
{
	delete m_VertexBuffer;
	delete m_IndexBuffer;
	delete m_VBLayout;
	delete m_VAO;

	//for (size_t i = 0; i < m_MaterialCount; i++)
	//	delete m_Materials[i];

	m_Vertices.clear();
	m_Indices.clear();
	m_MaterialCount = 0;
	m_Materials.clear();
}

void Renderer::SetShader(Shader& shader)
{
	if (!&shader) return;

	m_Shader = &shader;
}

void Renderer::SetVertices(Vertex* vertices, unsigned int count)
{
	m_Vertices = std::vector<Vertex>(vertices, vertices + count);
}

void Renderer::SetIndices(unsigned int* indices, unsigned int count)
{
	m_Indices = std::vector<unsigned int>(indices, indices + count);
}

void Renderer::AddMaterial(Material*& material)
{
	if (!material) return;

	for (size_t i = 0; i < m_MaterialCount; i++)
	{
		if (material->Id == m_Materials[i]->Id)
		{
			Log("Material already added");
			return;
		}
	}

	m_Materials.push_back(material);
	m_MaterialCount++;
}

void Renderer::RemoveMaterial(Material*& material)
{
	bool found = false;
	for (auto i = m_Materials.begin(); i != m_Materials.end(); i++)
	{
		if ((*i)->Id == material->Id)
		{
			found = true;
			auto tmp = *i;
			*i = *(m_Materials.end() - 1);
			*(m_Materials.end() - 1) = tmp;
			break;
		}
	}

	if (found)
		m_Materials.erase(m_Materials.end() - 1);
}

void Renderer::Render()
{
	BindResources();

	Prepare();

	Draw();

	UnBindResources();
}

void Renderer::BindResources()
{
	m_VAO->Bind();
}

void Renderer::UnBindResources()
{
	m_VAO->UnBind();
}

void Renderer::Prepare()
{
	auto window = WindowManager::getInstance()->GetCurrentWindow();
	auto camera = window->GetCameraManager().GetCamera();

	if (!camera) return;

	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * gameVastu->m_transform->GetTransformMatrix();

	std::vector<Vertex> vertices = m_Vertices;
	for (size_t i = 0; i < m_VertexCount; i++)
	{
		glm::vec4 pixelPosition(m_Vertices[i].Position.x, m_Vertices[i].Position.y, m_Vertices[i].Position.z, 1.0f);
		pixelPosition = mvp * pixelPosition;
		vertices[i].PixelPosition.x = pixelPosition.x;
		vertices[i].PixelPosition.y = pixelPosition.y;
		vertices[i].PixelPosition.z = pixelPosition.z;
		vertices[i].PixelPosition.w = pixelPosition.w;
	}

	m_VertexBuffer->Bind();
	GLLog(glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(Vertex), &vertices[0]));
	m_VertexBuffer->UnBind();


	for (auto material : m_Materials)
	{
		material->m_Shader->Bind();
		window->GetLightingManager().UpdateShaderLightData(*material->m_Shader, *camera->GetGameVastu()->m_transform, *gameVastu->m_transform);
		material->m_Shader->UnBind();
	}
}

void Renderer::Draw()
{
	if (m_MaterialCount < 1 || !m_Materials[0])
		return;

	m_Materials[0]->Bind();

	GLLog(glDrawElementsBaseVertex(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0, 0));
}

void Renderer::OnInspectorGUI()
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

	if (ImGui::CollapsingHeader("Material Info"))
	{
		ImGui::Indent();
		for (size_t i = 0; i < m_MaterialCount; i++)
		{
			std::string header = "Material " + std::to_string(i);
			if (ImGui::CollapsingHeader(header.c_str()))
			{
				ImGui::PushID(i);

				Material*& material = m_Materials[i];

				ImGui::ColorEdit3("Ambient Color", material->m_AmbientColor);
				ImGui::ColorEdit3("Diffuse Color", material->m_DiffuseColor);
				ImGui::ColorEdit3("Specular Color", material->m_SpecularColor);

				if (m_Materials[i]->m_DiffuseTexture)
				{
					ImGui::Text("Diffuse Texture: ");
					ImGui::Indent();
					ImGui::Text("Name: ");	ImGui::SameLine();
					ImGui::Text(material->m_DiffuseTexture->FileName.c_str());
					ImGui::Text("Path: ");	ImGui::SameLine();
					ImGui::Text(material->m_DiffuseTexture->FilePath.c_str());
					ImGui::Unindent();
				}

				if (m_Materials[i]->m_SpecularExponent)
				{
					ImGui::Text("Specular Exponent Texture: ");
					ImGui::Indent();
					ImGui::Text("Name: ");	ImGui::SameLine();
					ImGui::Text(material->m_SpecularExponent->FileName.c_str());
					ImGui::Text("Path: ");	ImGui::SameLine();
					ImGui::Text(material->m_SpecularExponent->FilePath.c_str());
					ImGui::Unindent();
				}

				ImGui::PopID();
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
				ImGui::DragFloat("X", &m_Vertices[i].Position.x, 0.01f, 0.0f, 1.0f);
				ImGui::TableNextColumn();
				ImGui::DragFloat("Y", &m_Vertices[i].Position.y, 0.01f, 0.0f, 1.0f);
				ImGui::TableNextColumn();
				ImGui::DragFloat("Z", &m_Vertices[i].Position.z, 0.01f, 0.0f, 1.0f);
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

	if (ImGui::CollapsingHeader("Normals"))
	{
		if (ImGui::BeginTable("Normals", 3))
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
				ImGui::DragFloat("X", &m_Vertices[i].Normal.x, 0.01f, 0.0f, 1.0f);
				ImGui::TableNextColumn();
				ImGui::DragFloat("Y", &m_Vertices[i].Normal.y, 0.01f, 0.0f, 1.0f);
				ImGui::TableNextColumn();
				ImGui::DragFloat("Z", &m_Vertices[i].Normal.z, 0.01f, 0.0f, 1.0f);
				ImGui::PopID();
			}
		}
		ImGui::EndTable();
	}
}