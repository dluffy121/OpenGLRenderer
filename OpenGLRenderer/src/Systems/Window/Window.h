#pragma once

#include <functional>
#include <GLFW/glfw3.h>
#include "../Utils/Action.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Renderer/Renderer.h"
#include "../GameVastu/GameVastu.h"
#include <imgui/imgui.h>
#include "../GUI/GUIWindow.h"

class Window
{
public:
	std::vector<Action> UpdateActions;

	glm::mat4 m_Projection;

private:
	std::string m_WindowId;
	GLFWwindow* m_GLFWWindow;
	ImGuiContext* m_ImGuiContext;

	int m_Width;
	int m_Height;

	VertexArray* m_VA;

	Camera* m_Camera;

	std::vector<const GameVastu*> m_GameVastus;
	std::vector<const Renderer*> m_Renderers;
	std::vector<const GUIWindow*> m_GUIWindows;

public:
	Window(const std::string& rendererId, int width = 640, int height = 480, GLFWwindow* sharedWindow = NULL, ImFontAtlas* sharedFontAtlas = NULL);
	~Window();

	void Clear() const;
	void Draw() const;

	inline Camera& GetCamera() const { return *m_Camera; }
	inline void SetCamera(Camera& camera) { m_Camera = &camera; }

	inline std::string GetWindowId() const { return m_WindowId; }
	inline GLFWwindow* GetGLFWWindow() { return m_GLFWWindow; }
	inline ImGuiContext* GetImGuiContext() { return m_ImGuiContext; }
	inline int GetWindowWidth() { return m_Width; }
	inline int GetWindowHeight() { return m_Height; }

	void RegisterGameVastu(const GameVastu& gameVastu);
	void UnRegisterGameVastu(const GameVastu& gameVastu);

	void RegisterGUIWindow(const GUIWindow& guiWindow);
	void UnRegisterGUIWindow(const GUIWindow& guiWindow);

private:
	void CreateImGUIContext(ImFontAtlas* sharedFontAtlas);

	void InstallCallbacks();
	void CursorEnterCallback(GLFWwindow* glfwWindow, int entered);
	void CursorPosCallback(GLFWwindow* glfwWindow, double x, double y);
	void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
	void ScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset);
	void KeyCallback(GLFWwindow* glfwWindow, int keycode, int scancode, int action, int mods);
	void CharCallback(GLFWwindow* glfwWindow, unsigned int c);
};