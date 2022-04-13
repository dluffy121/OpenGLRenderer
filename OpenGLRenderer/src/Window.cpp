#include "Window.h"
#include "OpenGLHelper.h"
#include <iostream>
#include "Math.h"
#include "GameVastuManager.h"

Window::Window(const std::string& rendererId, int width, int height, GLFWwindow* sharedWindow) :
	m_WindowId(rendererId),
	m_Width(width),
	m_Height(height),
	m_Camera(nullptr),
	m_VA(new VertexArray())
{
	m_GLFWWindow = OpenGLHelper::CreateWindow(width, height, m_WindowId, sharedWindow);

	m_Projection = glm::ortho(0.0f, (float)GetWindowWidth(), 0.0f, (float)GetWindowHeight(), -1.0f, 1.0f);	// Providing int to this method does not produce desired output

	if (!m_GLFWWindow)
	{
		OpenGLHelper::TerminateGLFW();
		ASSERT(false);
		return;
	}
}

Window::~Window()
{
	OpenGLHelper::DestroyGLFWWindow(m_GLFWWindow);
	std::cout << "Deleted Window with id: " << m_WindowId << std::endl;
}

void Window::Clear() const
{
	GLLog(glClear(GL_COLOR_BUFFER_BIT));
}

void Window::Draw() const
{
	m_VA->Bind();

	for (auto& renderer : m_Renderers)
	{
		GameVastu* gameVastu = GameVastuManager::getInstance()->FindGameVastu(renderer->GameVastuId);
		if (gameVastu == nullptr)
			continue;

		glm::mat4 mvp = m_Projection
			* m_Camera->m_View
			* gameVastu->m_transform->GetModelMatrix();

		if (!renderer->UpdateShaderMVP(mvp))
			continue;

		RenderData* renderData = renderer->GetRenderData();

		renderer->BindShader();
		renderer->BindTexture();

		renderData->m_ib->Bind();

		GLLog(glDrawElements(GL_TRIANGLES, renderData->m_ib->GetCount(), renderData->m_ib->GetIndexType(), nullptr));						// this method will draw from binded element buffer array https://docs.gl/gl4/glDrawElements

		renderData->m_ib->UnBind();

		renderer->UnBindTexture();
		renderer->UnBindShader();
	}

	m_VA->UnBind();
}

void Window::RegisterGameVastu(const GameVastu& gameVastu)
{
	unsigned int id = gameVastu.GetId();

	for (auto it = m_GameVastus.begin(); it != m_GameVastus.end(); ++it)
		if ((*it)->GetId() == id)
			return;

	m_GameVastus.push_back(&gameVastu);

	Renderer* renderer = gameVastu.GetComponent<Renderer>();
	if (renderer == nullptr)
		return;
	id = renderer->GetId();

	for (auto it = m_Renderers.begin(); it != m_Renderers.end(); ++it)
		if ((*it)->GetId() == id)
			return;

	m_Renderers.push_back(gameVastu.GetComponent<Renderer>());

	renderer->BindToVA(*m_VA);
}

void Window::UnRegisterGameVastu(const GameVastu& gameVastu)
{
	unsigned int id = gameVastu.GetId();

	for (auto it = m_GameVastus.begin(); it != m_GameVastus.end(); ++it)
		if ((*it)->GetId() == id)
			m_GameVastus.erase(it);

	Renderer* renderer = gameVastu.GetComponent<Renderer>();
	if (renderer == nullptr)
		return;
	id = renderer->GetId();

	for (auto it = m_Renderers.begin(); it != m_Renderers.end(); ++it)
		if ((*it)->GetId() == id)
			m_Renderers.erase(it);
}