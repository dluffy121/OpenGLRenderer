#include "Renderer.h"
#include "OpenGLHelper.h"
#include <iostream>

Renderer::Renderer(const std::string& rendererId, int width, int height, GLFWwindow* sharedWindow) :
	m_RendererId(rendererId),
	m_RenderData()
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
	if (m_RenderData.m_VertexArray == nullptr)
	{
		std::cout << "VertexArray is null" << std::endl;
		ASSERT(false);
		return;
	}

	if (m_RenderData.m_IndexBuffer == nullptr)
	{
		std::cout << "IndexBufferis null" << std::endl;
		ASSERT(false);
		return;
	}

	if (m_RenderData.m_Shader == nullptr)
	{
		std::cout << "Shader is null" << std::endl;
		ASSERT(false);
		return;
	}

	m_RenderData.m_Shader->Bind();
	m_RenderData.m_VertexArray->Bind();
	m_RenderData.m_IndexBuffer->Bind();

	GLLog(glDrawElements(GL_TRIANGLES, m_RenderData.m_IndexBuffer->GetCount(), m_RenderData.m_IndexBuffer->GetIndexType(), nullptr));						// this method will draw from binded element buffer array https://docs.gl/gl4/glDrawElements

	m_RenderData.m_VertexArray->UnBind();
	m_RenderData.m_IndexBuffer->UnBind();
	m_RenderData.m_Shader->UnBind();
}