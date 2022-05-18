#include "../../Core/GL/OpenGLHelper.h"
#include "WindowManager.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

WindowManager::WindowManager() {}

WindowManager* WindowManager::getInstance()
{
	static WindowManager instance;
	return &instance;
}

Window* WindowManager::GetWindowInstance(const std::string& windowId, int width, int height, GLFWwindow* sharedWindow, ImFontAtlas* sharedFontAtlas)
{
	for (int i = 0; i < rendererCount; i++)
	{
		if (m_WindowCollection[i]->GetWindowId() == windowId)
			return m_WindowCollection[i];
	}

	Window* window = new Window(windowId, width, height, sharedWindow, sharedFontAtlas);
	m_WindowCollection.push_back(window);

	GLFWwindow* glfwWindow = window->GetGLFWWindow();
	m_GLFWWindowCollection.push_back(glfwWindow);

	ImGuiContext* imGuiContext = window->GetImGuiContext();
	m_ImGuiContextCollection.push_back(imGuiContext);

	rendererCount++;

	return window;
}

void WindowManager::RenderLoop()
{
	int i = 0;
	bool windowClosed = false;
	for (;;)
	{
		if (rendererCount == 0)
			exit(1);

		glfwPollEvents();

		for (; i < rendererCount; i++)
		{
			glfwMakeContextCurrent(m_GLFWWindowCollection[i]);
			ImGui::SetCurrentContext(m_ImGuiContextCollection[i]);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			m_WindowCollection[i]->Clear();

			for (auto& raction : UpdateActionsRenderer)
				raction(*m_WindowCollection[i]);

			m_WindowCollection[i]->Draw();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(m_GLFWWindowCollection[i]);

			if (glfwWindowShouldClose(m_GLFWWindowCollection[i]))
			{
				windowClosed = true;
				OpenGLHelper::DestroyGLFWWindow(m_GLFWWindowCollection[i]);
				m_GLFWWindowCollection.erase(m_GLFWWindowCollection.begin() + i);
				m_WindowCollection.erase(m_WindowCollection.begin() + i);
				m_ImGuiContextCollection.erase(m_ImGuiContextCollection.begin() + i);
				rendererCount--;
				i--;
			}
		}

		if (!windowClosed)
			for (auto& action : UpdateActions)
				action();

		windowClosed = false;
		i = 0;
	}
}