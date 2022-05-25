#include "../../Core/GL/OpenGLHelper.h"
#include "WindowManager.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

WindowManager::WindowManager() :
	rendererCount(0),
	m_currentWindow(NULL),
	m_initialized(false)
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

	if (m_initialized)
	{
		ImGuiContext* context = ImGui::GetCurrentContext();
		ImGui::SetCurrentContext(m_currentWindow->GetImGuiContext());
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetCurrentContext(window->GetImGuiContext());
		ImGui_ImplOpenGL3_Init(io);
		ImGui::SetCurrentContext(context);
	}

	return window;
}

void WindowManager::InitImGuiRendererBackend()
{
	if (!rendererCount)
		return;

	ImGui::SetCurrentContext(m_WindowCollection[0]->GetImGuiContext());
	ImGuiContext* context = ImGui::GetCurrentContext();
	const char* glsl_version = "#version 460";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGuiIO& io = ImGui::GetIO();

	GLsizei size = rendererCount;
	for (size_t i = 1; i < size; i++)
	{
		ImGui::SetCurrentContext(m_WindowCollection[i]->GetImGuiContext());

		ImGui_ImplOpenGL3_Init(io);
	}

	ImGui::SetCurrentContext(context);
}

void WindowManager::ShutdownImGuiRendererBackend()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void WindowManager::Init()
{
	InitImGuiRendererBackend();

	m_initialized = true;
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

void WindowManager::Exit()
{
	ShutdownImGuiRendererBackend();
}

GameVastu* WindowManager::SelectedGameVastu(GameVastu* gameVastu)
{
	if (gameVastu)
		m_currentWindow->SelectedGameVastu = gameVastu;

	return m_currentWindow->SelectedGameVastu;
}