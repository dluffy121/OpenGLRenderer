#include "Renderer.h"
#include <GameVastu/GameVastu.h>
#include <Logger/Logger.h>
#include <Camera/Camera.h>

using namespace core;
using namespace core::gl;

Renderer::Renderer(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount) :
	m_Vertices(vertices),
	m_VertexCount(vertexCount),
	m_Indices(indices),
	m_IndexCount(indexCount),
	m_TriangleCount(indexCount / 3),
	_Vertices(NULL),
	_Indices(NULL),
	m_Shader(NULL)
{}

Renderer::~Renderer()
{}

void Renderer::SetShader(Shader& shader)
{
	m_Shader = &shader;
}

void Renderer::AddTexture(int samplerId, Texture& texture)
{
	m_Textures[samplerId] = &texture;
}

void Renderer::RemoveTexture(int samplerId)
{
	m_Textures.erase(samplerId);
}

bool Renderer::BindShader() const
{
	if (m_Shader == nullptr)
		return false;

	m_Shader->Bind();
	return true;
}

bool Renderer::UnBindShader() const
{
	if (m_Shader == nullptr)
		return false;

	m_Shader->UnBind();
	return true;
}

void Renderer::BindTextures() const
{
	for (auto& sampler : m_Textures)
		sampler.second->BindToUnit(sampler.first);
}

void Renderer::UnBindTexture() const
{
	for (auto& sampler : m_Textures)
		sampler.second->UnBindFromUnit(sampler.first);
}

void Renderer::Render()
{
	BindTextures();

	auto window = WindowManager::getInstance()->GetCurrentWindow();
	auto camera = window->GetCameraManager().GetCamera();

	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * gameVastu->m_transform->GetTransformMatrix();

	_Vertices = CopyArray(m_Vertices, m_VertexCount);

	for (size_t i = 0; i < m_VertexCount; i++)
	{
		glm::vec4 pixelPosition(m_Vertices[i].Position.x, m_Vertices[i].Position.y, m_Vertices[i].Position.z, 1.0f);
		pixelPosition = mvp * pixelPosition;
		_Vertices[i].PixelPosition.x = pixelPosition.x;
		_Vertices[i].PixelPosition.y = pixelPosition.y;
		_Vertices[i].PixelPosition.z = pixelPosition.z;
		_Vertices[i].PixelPosition.w = pixelPosition.w;
	}

	window->GetBatchRenderer()->Draw(*m_Shader, _Vertices, m_VertexCount, m_Indices, m_IndexCount);

	delete[] _Vertices;
	delete[] _Indices;
}

void Renderer::OnInspectorGUI()
{
	if (ImGui::CollapsingHeader("Render Info"))
	{
		ImGui::Text("Vertex Count: ");
		ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
		ImGui::Text(std::to_string(m_VertexCount).c_str());
		ImGui::Text("Index Count: ");
		ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
		ImGui::Text(std::to_string(m_IndexCount).c_str());
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

	if (m_Textures.size() > 0)
		if (ImGui::CollapsingHeader("Texture Info"))
		{
			for (auto& texture : m_Textures)
			{
				ImGui::Text("Sampler Id: ");
				ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
				ImGui::Text(std::to_string(texture.first).c_str());
				ImGui::Text("Texture Id: ");
				ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
				ImGui::Text(std::to_string(texture.second->Id).c_str());
				ImGui::Text("Texture Path: ");
				ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
				ImGui::Text(texture.second->FilePath.c_str());
			}
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