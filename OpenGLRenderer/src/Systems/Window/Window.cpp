#include "../../Core/GL/OpenGLHelper.h"
#include "Window.h"
#include <GameVastu/GameVastuManager.h>
#include <Component/Camera/Camera.h>
#include <GUI/GUIWindow.h>
#include <imgui/imgui_impl_glfw.h>
#include "../../Core/Logger.h"

Window::Window(const std::string& rendererId, int width, int height, GLFWwindow* sharedWindow, ImFontAtlas* sharedFontAtlas) :
	m_WindowId(rendererId),
	m_Width(width),
	m_Height(height),
	SelectedGameVastu(NULL)
{
	m_GLFWWindow = OpenGLHelper::CreateWindow(width, height, m_WindowId, sharedWindow);
	glfwMakeContextCurrent(m_GLFWWindow);

	glfwSetWindowUserPointer(m_GLFWWindow, this);

	CreateImGUIContext(sharedFontAtlas);

	InstallCallbacks();

	m_CameraVastu = GameVastuManager::getInstance()->CreateGameVastu();
	m_Camera = new Camera(m_Width, m_Height);
	m_CameraVastu->AddComponent(*m_Camera);

	m_VAO = new VertexArray();

	if (!m_GLFWWindow)
	{
		OpenGLHelper::TerminateGLFW();
		ASSERT(false);
		return;
	}
}

Window::~Window()
{
	for (auto& scene : m_Scenes)
		delete scene.second;

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

void Window::NewFrame()
{
	m_Frames++;
}

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void Window::Clear()
{
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	GLLog(glClear(GL_COLOR_BUFFER_BIT));
}

void Window::Update()
{
	for (auto& component : m_Components)
	{
		component->Update();
	}
}

void Window::Render()
{
	m_VAO->Bind();

	glm::mat4 vp = m_Camera->GetProjectionMatrix()
		* m_Camera->GetViewMatrix();

	for (auto& component : m_Components)
	{
		component->Render(vp);
	}

	m_VAO->UnBind();
}

void Window::RenderGUI()
{
	for (auto& component : m_Components)
	{
		component->OnGUI();
	}

	for (auto& guiWindow : m_GUIWindows)
	{
		guiWindow->Draw(this);
	}
}

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

void Window::RegisterComponent(Component& component)
{
	if (std::find(m_Components.begin(), m_Components.end(), &component) != m_Components.end())
		return;

	component.Awake();

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
