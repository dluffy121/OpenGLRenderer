#include "Renderer.h"
#include "OpenGLHelper.h"
#include <iostream>

Renderer::Renderer(const std::string& rendererId, int width, int height, GLFWwindow* sharedWindow) :
	m_RendererId(rendererId)
{
	m_Window = OpenGLHelper::CreateWindow(width, height, m_RendererId, sharedWindow);
	if (!m_Window)
	{
		OpenGLHelper::TerminateGLFW();
		ASSERT(false);
		return;
	}
}

Renderer::~Renderer()
{
	OpenGLHelper::DestroyWindow(m_Window);
	std::cout << "Deleted Renderer with id: " << m_RendererId << std::endl;
}

void Renderer::Clear() const
{
	GLLog(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw() const
{
	m_RenderData.VertexArray->Bind();
	m_RenderData.IndexBuffer->Bind();
	for (auto& shader : m_RenderData.Shaders)
		shader->Bind();
	for (auto& texture : m_RenderData.Textures)
		texture->Bind();

	GLLog(glDrawElements(GL_TRIANGLES, m_RenderData.IndexBuffer->GetCount(), m_RenderData.IndexBuffer->GetIndexType(), nullptr));						// this method will draw from binded element buffer array https://docs.gl/gl4/glDrawElements

	m_RenderData.VertexArray->UnBind();
	m_RenderData.IndexBuffer->UnBind();
	for (auto& shader : m_RenderData.Shaders)
		shader->UnBind();
	for (auto& texture : m_RenderData.Textures)
		texture->UnBind();
}