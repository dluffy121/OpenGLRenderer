#include "../Core/GL/OpenGLHelper.h"
#include "Window.h"
#include <iostream>
#include "Math.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "../GameVastu/GameVastuManager.h"

Window::Window(const std::string& rendererId, int width, int height, GLFWwindow* sharedWindow, ImFontAtlas* sharedFontAtlas) :
	m_WindowId(rendererId),
	m_Width(width),
	m_Height(height),
	m_Camera(nullptr),
	m_VA(new VertexArray())
{
	m_GLFWWindow = OpenGLHelper::CreateWindow(width, height, m_WindowId, sharedWindow);
	glfwMakeContextCurrent(m_GLFWWindow);

	glfwSetWindowUserPointer(m_GLFWWindow, this);

	CreateImGUIContext(sharedFontAtlas);

	InstallCallbacks();

	m_Projection = glm::ortho(0.0f, (float)GetWindowWidth(), 0.0f, (float)GetWindowHeight(), -999.0f, 999.0f);	// Providing int to this method does not produce desired output

	if (!m_GLFWWindow)
	{
		OpenGLHelper::TerminateGLFW();
		ASSERT(false);
		return;
	}
}

Window::~Window()
{
	ImGui::SetCurrentContext(m_ImGuiContext);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	OpenGLHelper::DestroyGLFWWindow(m_GLFWWindow);
	std::cout << "Deleted Window with id: " << m_WindowId << std::endl;
}

void Window::CreateImGUIContext(ImFontAtlas* sharedFontAtlas)
{
	m_ImGuiContext = ImGui::CreateContext(sharedFontAtlas);
	ImGui::SetCurrentContext(m_ImGuiContext);
	ImGui_ImplGlfw_InitForOpenGL(m_GLFWWindow, false);
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 460";
	ImGui_ImplOpenGL3_Init(glsl_version);
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
			* m_Camera->gameVastu->m_transform->GetPositionMatrix()
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

	for (auto& guiWindow : m_GUIWindows)
	{
		guiWindow->Draw();
	}

	m_VA->UnBind();
}

void Window::InstallCallbacks()
{
	auto cursorEnterFunc = [](GLFWwindow* glfwWindow, int entered)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->CursorEnterCallback(glfwWindow, entered);
	};
	auto cursorPosFunc = [](GLFWwindow* glfwWindow, double x, double y)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->CursorPosCallback(glfwWindow, x, y);
	};
	auto mouseButtonFunc = [](GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->MouseButtonCallback(glfwWindow, button, action, mods);
	};
	auto scrollFunc = [](GLFWwindow* glfwWindow, double xoffset, double yoffset)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->ScrollCallback(glfwWindow, xoffset, yoffset);
	};
	auto keyFunc = [](GLFWwindow* glfwWindow, int keycode, int scancode, int action, int mods)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->KeyCallback(glfwWindow, keycode, scancode, action, mods);
	};
	auto charFunc = [](GLFWwindow* glfwWindow, unsigned int c)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->CharCallback(glfwWindow, c);
	};

	glfwSetCursorEnterCallback(m_GLFWWindow, cursorEnterFunc);
	glfwSetCursorPosCallback(m_GLFWWindow, cursorPosFunc);
	glfwSetMouseButtonCallback(m_GLFWWindow, mouseButtonFunc);
	glfwSetScrollCallback(m_GLFWWindow, scrollFunc);
	glfwSetKeyCallback(m_GLFWWindow, keyFunc);
	glfwSetCharCallback(m_GLFWWindow, charFunc);
}

void Window::CursorEnterCallback(GLFWwindow* glfwWindow, int entered)
{
	ImGuiContext* lastImGuiContext = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(m_ImGuiContext);

	ImGui_ImplGlfw_CursorEnterCallback(glfwWindow, entered);

	ImGui::SetCurrentContext(lastImGuiContext);
}

void Window::CursorPosCallback(GLFWwindow* glfwWindow, double x, double y)
{
	ImGuiContext* lastImGuiContext = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(m_ImGuiContext);

	ImGui_ImplGlfw_CursorPosCallback(glfwWindow, x, y);

	ImGui::SetCurrentContext(lastImGuiContext);
}

void Window::MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
{
	ImGuiContext* lastImGuiContext = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(m_ImGuiContext);

	ImGui_ImplGlfw_MouseButtonCallback(glfwWindow, button, action, mods);

	ImGui::SetCurrentContext(lastImGuiContext);
}

void Window::ScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset)
{
	ImGuiContext* lastImGuiContext = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(m_ImGuiContext);

	ImGui_ImplGlfw_ScrollCallback(glfwWindow, xoffset, yoffset);

	ImGui::SetCurrentContext(lastImGuiContext);
}

void Window::KeyCallback(GLFWwindow* glfwWindow, int keycode, int scancode, int action, int mods)
{
	ImGuiContext* lastImGuiContext = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(m_ImGuiContext);

	ImGui_ImplGlfw_KeyCallback(glfwWindow, keycode, scancode, action, mods);

	ImGui::SetCurrentContext(lastImGuiContext);
}


void Window::CharCallback(GLFWwindow* glfwWindow, unsigned int c)
{
	ImGuiContext* lastImGuiContext = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(m_ImGuiContext);

	ImGui_ImplGlfw_CharCallback(glfwWindow, c);

	ImGui::SetCurrentContext(lastImGuiContext);
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

void Window::RegisterGUIWindow(const GUIWindow& guiWindow)
{
	if (std::find(m_GUIWindows.begin(), m_GUIWindows.end(), &guiWindow) != m_GUIWindows.end())
		return;

	m_GUIWindows.push_back(&guiWindow);
}

void Window::UnRegisterGUIWindow(const GUIWindow& guiWindow)
{
	size_t size = m_GUIWindows.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_GUIWindows[i] == &guiWindow)
		{
			const GUIWindow* tmp = m_GUIWindows[size - 1];
			m_GUIWindows[size - 1] = m_GUIWindows[i];
			m_GUIWindows[i] = tmp;
			size--;
			break;
		}
	}

	m_GUIWindows.erase(m_GUIWindows.end());
}
