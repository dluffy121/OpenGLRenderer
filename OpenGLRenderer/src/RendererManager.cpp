#include "RendererManager.h"
#include <iostream>

RendererManager::RendererManager() {}

RendererManager* RendererManager::getInstance()
{
	static RendererManager instance;
	return &instance;
}

Renderer* RendererManager::GetRendererInstance(const std::string& rendererId, int width, int height, GLFWwindow* sharedWindow)
{
	for (int i = 0; i < rendererCount; i++)
	{
		if (m_RendererCollection[i]->GetRendererId() == rendererId)
			return m_RendererCollection[i];
	}

	Renderer* renderer = new Renderer(rendererId, width, height, sharedWindow);
	m_RendererCollection.push_back(renderer);

	GLFWwindow* window = renderer->GetRendererWindow();
	m_WindowCollection.push_back(window);

	rendererCount++;

	return renderer;
}

void RendererManager::RenderLoop()
{
	int i = 0;
	bool windowClosed = false;
	for (;;)
	{
		if (rendererCount == 0)
			exit(1);

		for (; i < rendererCount; i++)
		{
			glfwMakeContextCurrent(m_WindowCollection[i]);

			m_RendererCollection[i]->Clear();

			for (auto& raction : UpdateActionsRenderer)
				raction(*m_RendererCollection[i]);

			m_RendererCollection[i]->Draw();

			glfwSwapBuffers(m_WindowCollection[i]);

			if (glfwWindowShouldClose(m_WindowCollection[i]))
			{
				windowClosed = true;
				OpenGLHelper::DestroyWindow(m_WindowCollection[i]);
				m_WindowCollection.erase(m_WindowCollection.begin() + i);
				m_RendererCollection.erase(m_RendererCollection.begin() + i);
				rendererCount--;
				i--;
			}
		}

		if (!windowClosed)
			for (auto& action : UpdateActions)
				action();

		windowClosed = false;
		i = 0;
		glfwPollEvents();
	}
}
