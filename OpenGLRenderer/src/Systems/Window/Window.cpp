#include "Window.h"
#include <GameVastu/GameVastuManager.h>
#include <Camera/Camera.h>
#include <GUI/GUIWindow.h>
#include <imgui/imgui_impl_glfw.h>
#include <Logger/Logger.h>
#include <algorithm>
#include <iterator>
#include <imgui/imgui_impl_opengl3.h>
#include <RenderIntent/StandardRenderIntent.h>
#include <RenderIntent/BatchRenderIntent.h>

using namespace core;
using namespace core::gl;

Window::Window(const std::string& rendererId, int width, int height, GLFWwindow* sharedWindow, ImFontAtlas* sharedFontAtlas) :
	m_WindowId(rendererId),
	m_Width(width),
	m_Height(height),
	m_CameraManager(new CameraManager()),
	m_LightingManager(new LightingManager()),
	//m_RenderIntent(new BatchRenderIntent()),
	m_RenderIntent(new StandardRenderIntent()),
	SelectedGameVastu(NULL)
{
	GLFWmonitor* monitor = NULL;

	if (m_Width == 0 || m_Height == 0)
	{
		monitor = glfwGetPrimaryMonitor();
		GetPrimaryMonitorSize(monitor, m_Width, m_Height);
		m_IsFullscreen = true;
	}

	m_GLFWWindow = OpenGLHelper::CreateWindow(m_Width, m_Height, m_WindowId, sharedWindow, monitor);
	glfwMakeContextCurrent(m_GLFWWindow);

	glfwSetWindowUserPointer(m_GLFWWindow, this);

	CreateImGUIContext(sharedFontAtlas);

	InstallCallbacks();

	if (!m_GLFWWindow)
	{
		OpenGLHelper::TerminateGLFW();
		ASSERT(false, "Failed to create GLFW window");
		return;
	}

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	for (auto& scene : m_Scenes)
		delete scene.second;

	if (m_RenderIntent) delete m_RenderIntent;
	delete m_CameraManager;
	delete m_LightingManager;

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	OpenGLHelper::DestroyGLFWWindow(m_GLFWWindow);
	std::cout << "Deleted Window with id: " << m_WindowId << std::endl;
}

void Window::NewFrame()
{
	m_Frames++;
}

void Window::EndFrame()
{
	m_RenderIntent->Reset();
	m_ScrollOffset[0] = 0.0f; m_ScrollOffset[1] = 0.0f;
}

ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);

void Window::Clear()
{
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	GLLog(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Window::Update()
{
	for (auto& scene : m_Scenes)
	{
		scene.second->Update();
	}

	for (auto& component : m_Components)
	{
		component->_Update();
	}
}

void Window::Render()
{
	if (!m_CameraManager->GetCamera()) return;

	for (auto& component : m_Components)
	{
		component->_Render();
	}

	m_RenderIntent->Draw();
}

void Window::RenderGUI()
{
	ImGui::SetCurrentContext(m_ImGuiContext);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto& component : m_Components)
	{
		component->_OnGUI();
	}

	for (auto& guiWindow : m_GUIWindows)
	{
		guiWindow->Draw(this);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#pragma region GLFW Window

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
	auto resizeFunc = [](GLFWwindow* glfwWindow, int cx, int cy)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->ResizeCallback(glfwWindow, cx, cy);
	};

	glfwSetCursorEnterCallback(m_GLFWWindow, cursorEnterFunc);
	glfwSetCursorPosCallback(m_GLFWWindow, cursorPosFunc);
	glfwSetMouseButtonCallback(m_GLFWWindow, mouseButtonFunc);
	glfwSetScrollCallback(m_GLFWWindow, scrollFunc);
	glfwSetKeyCallback(m_GLFWWindow, keyFunc);
	glfwSetCharCallback(m_GLFWWindow, charFunc);
	glfwSetWindowSizeCallback(m_GLFWWindow, resizeFunc);
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
	m_ScrollOffset[0] = xoffset;
	m_ScrollOffset[1] = yoffset;

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

void Window::ResizeCallback(GLFWwindow* glfwWindow, int cx, int cy)
{
	m_ResizeViewport = true;
}

bool Window::GetKeyPressed(int keyCode)
{
	return glfwGetKey(m_GLFWWindow, keyCode) == GLFW_PRESS;
}

bool Window::GetKeyDown(int keyCode)
{
	return glfwGetKey(m_GLFWWindow, keyCode) == GLFW_REPEAT;
}

bool Window::GetKeyUp(int keyCode)
{
	return glfwGetKey(m_GLFWWindow, keyCode) == GLFW_RELEASE;
}

Vec2 Window::GetMousePos()
{
	double x, y;
	glfwGetCursorPos(m_GLFWWindow, &x, &y);
	return Vec2{ static_cast<float>(x), static_cast<float>(y) };
}

Vec2 Window::GetScroll()
{
	return Vec2{ static_cast<float>(m_ScrollOffset[0]), static_cast<float>(m_ScrollOffset[1]) };
}

void Window::EnableFullScreen()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (!monitor) return;

	GetPrimaryMonitorSize(glfwGetPrimaryMonitor(), m_Width, m_Height);

	glfwSetWindowMonitor(m_GLFWWindow, monitor, 0, 0, m_Width, m_Height, GLFW_DONT_CARE);

	m_IsFullscreen = true;
	m_ResizeViewport = true;
}

void Window::EnableWindowed()
{
	glfwSetWindowMonitor(m_GLFWWindow, NULL, 50, 50, m_Width - 100, m_Height - 100, GLFW_DONT_CARE);

	m_IsFullscreen = false;
	m_ResizeViewport = true;
}

void Window::GetPrimaryMonitorSize(GLFWmonitor* monitor, int& width, int& height) const
{
	if (!monitor) return;

	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	width = videoMode->width;
	height = videoMode->height;
}

#pragma endregion

#pragma region ImGui

void Window::CreateImGUIContext(ImFontAtlas* sharedFontAtlas)
{
	m_ImGuiContext = ImGui::CreateContext(sharedFontAtlas);
	ImGui::SetCurrentContext(m_ImGuiContext);
	ImGui_ImplGlfw_InitForOpenGL(m_GLFWWindow, false);
	ImGui::StyleColorsDark();
}

#pragma endregion

#pragma region Components

void Window::RegisterComponent(Component& component)
{
	if (std::find(m_Components.begin(), m_Components.end(), &component) != m_Components.end())
		return;

	component._Awake();
	if (component.GetEnabled())
		component._OnEnable();

	m_Components.push_back(&component);
}

void Window::UnRegisterComponent(Component& component)
{
	size_t size = m_Components.size();

	if (size == 0)
		return;

	for (size_t i = 0; i < size; ++i)
	{
		if (m_Components[i] == &component)
		{
			Component* tmp = m_Components[size - 1];
			m_Components[size - 1] = m_Components[i];
			m_Components[i] = tmp;
			size--;
			break;
		}
	}

	m_Components.erase(m_Components.end() - 1);
}

#pragma endregion

#pragma region GUI Windows

void Window::RegisterGUIWindow(GUIWindow& window)
{
	if (std::find(m_GUIWindows.begin(), m_GUIWindows.end(), &window) != m_GUIWindows.end())
		return;

	m_GUIWindows.push_back(&window);
}

void Window::UnRegisterGUIWindow(GUIWindow& window)
{
	size_t size = m_GUIWindows.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_GUIWindows[i] == &window)
		{
			GUIWindow* tmp = m_GUIWindows[size - 1];
			m_GUIWindows[size - 1] = m_GUIWindows[i];
			m_GUIWindows[i] = tmp;
			size--;
			break;
		}
	}

	m_GUIWindows.erase(m_GUIWindows.end() - 1);
}

#pragma endregion

#pragma region Scene

void Window::AddScene(scene::Scene* scene)
{
	std::string name = scene->GetName();

	if (m_Scenes.find(name) != m_Scenes.end())
		return;

	m_Scenes[name] = scene;
}

void Window::RemoveScene(const std::string& sceneName)
{
	if (m_Scenes.find(sceneName) == m_Scenes.end())
		return;

	delete m_Scenes[sceneName];
	m_Scenes.erase(sceneName);
}

void Window::RemoveScene(const scene::Scene& scene)
{
	RemoveScene(scene.GetName());
}

bool Window::IsSceneActive(const std::string& sceneName)
{
	return m_Scenes.find(sceneName) != m_Scenes.end();
}

bool Window::IsSceneActive(const scene::Scene& scene)
{
	return IsSceneActive(scene.GetName());
}

#pragma endregion