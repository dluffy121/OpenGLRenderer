#include "../../Core/GL/OpenGLHelper.h"
#include "WindowManager.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

WindowManager::WindowManager() :
	rendererCount(0),
	m_currentWindow(NULL),
	m_imGuiRendererInitialized(false)
{}

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

	if (!m_currentWindow)
		m_currentWindow = window;

	m_WindowCollection.push_back(window);

	rendererCount++;

	return window;
}

void WindowManager::InitImGuiRenderer()
{
	if (m_imGuiRendererInitialized)
	{
		ImGuiContext* context = ImGui::GetCurrentContext();
		ImGui::SetCurrentContext(m_WindowCollection[0]->GetImGuiContext());
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetCurrentContext(context);

		ImGui_ImplOpenGL3_Init(io);

		return;
	}

	const char* glsl_version = "#version 460";
	ImGui_ImplOpenGL3_Init(glsl_version);
	m_imGuiRendererInitialized = true;
}

void WindowManager::ShutdownImGuiRenderer()
{
	if (!m_imGuiRendererInitialized)
		return;

	if (!m_WindowCollection.empty())
		return;

	ImGui_ImplOpenGL3_Shutdown();
	m_imGuiRendererInitialized = false;
}

void WindowManager::WindowLoop()
{
	GLFWwindow* glfwWindow;
	int i = 0;
	bool windowClosed = false;
	for (;;)
	{
		if (rendererCount == 0)
			exit(1);

		glfwPollEvents();

		for (; i < rendererCount; i++)
		{
			m_currentWindow = m_WindowCollection[i];
			glfwWindow = m_currentWindow->GetGLFWWindow();

			glfwMakeContextCurrent(glfwWindow);

			m_currentWindow->Clear();

			m_currentWindow->NewFrame();

			m_currentWindow->Update();
			m_currentWindow->Render();

			ImGui::SetCurrentContext(m_currentWindow->GetImGuiContext());

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			for (auto& raction : UpdateActionsRenderGUI)
				raction(*m_currentWindow);

			m_currentWindow->RenderGUI();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(glfwWindow);

			if (glfwWindowShouldClose(glfwWindow))
			{
				windowClosed = true;
				delete m_currentWindow;
				m_WindowCollection[i] = m_WindowCollection[rendererCount - 1];
				m_WindowCollection[rendererCount - 1] = m_currentWindow;
				m_WindowCollection.erase(m_WindowCollection.end() - 1);
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