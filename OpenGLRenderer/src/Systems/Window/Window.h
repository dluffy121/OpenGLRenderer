#pragma once

#include <GL/OpenGLHelper.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <imgui/imgui.h>
#include <Action.h>
#include <GL/VertexArray/VertexArray.h>
#include "../Scene/Scene.h"
#include <Math/Math.h>
#include <Camera/CameraManager.h>
#include <Light/LightingManager.h>
#include <RenderIntent/RenderIntent.h>

class GameVastu;
class Component;
class Camera;
class GUIWindow;

class Window
{
private:
	std::string m_WindowId;

	int m_Width;
	int m_Height;

	unsigned int m_Frames;

public:
	Window(const std::string& rendererId, int width = 640, int height = 480, GLFWwindow* sharedWindow = NULL, ImFontAtlas* sharedFontAtlas = NULL);
	~Window();

	void NewFrame();
	void EndFrame();
	void Clear();
	void Update();
	void Render();
	void RenderGUI();

	inline std::string GetWindowId() const { return m_WindowId; }
	inline int GetWindowWidth() { return m_Width; }
	inline int GetWindowHeight() { return m_Height; }
	inline int GetFrames() { return m_Frames; }

#pragma region GLFW Window

private:
	GLFWwindow* m_GLFWWindow;

	double m_ScrollOffset[2];

public:
	inline GLFWwindow* GetGLFWWindow() const { return m_GLFWWindow; }
	bool GetKeyPressed(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);
	core::Vec2 GetMousePos();
	core::Vec2 GetScroll();

private:
	void InstallCallbacks();
	void CursorEnterCallback(GLFWwindow* glfwWindow, int entered);
	void CursorPosCallback(GLFWwindow* glfwWindow, double x, double y);
	void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
	void ScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset);
	void KeyCallback(GLFWwindow* glfwWindow, int keycode, int scancode, int action, int mods);
	void CharCallback(GLFWwindow* glfwWindow, unsigned int c);

#pragma endregion

#pragma region RenderIntent

private:
	RenderIntent* m_RenderIntent;

public:
	inline RenderIntent& GetRenderIntent() { return *m_RenderIntent; }

#pragma endregion

#pragma region ImGui

private:
	ImGuiContext* m_ImGuiContext;

public:
	inline ImGuiContext* GetImGuiContext() const { return m_ImGuiContext; }

private:
	void CreateImGUIContext(ImFontAtlas* sharedFontAtlas);

#pragma endregion

#pragma region Camera

private:
	CameraManager* m_CameraManager;

public:
	inline CameraManager& GetCameraManager() const { return *m_CameraManager; }

#pragma endregion

#pragma region Selection

public:
	GameVastu* SelectedGameVastu;

#pragma endregion

#pragma region Components

private:
	std::vector<Component*> m_Components;

public:
	void RegisterComponent(Component& component);
	void UnRegisterComponent(Component& component);

#pragma endregion

#pragma region GUI Windows

private:
	std::vector<GUIWindow*> m_GUIWindows;

public:
	void RegisterGUIWindow(GUIWindow& window);
	void UnRegisterGUIWindow(GUIWindow& window);

#pragma endregion

#pragma region Scene

private:
	std::unordered_map<std::string, scene::Scene*> m_Scenes;

public:
	void AddScene(scene::Scene* scene);
	void RemoveScene(const std::string& sceneName);
	void RemoveScene(const scene::Scene& scene);
	bool IsSceneActive(const std::string& sceneName);
	bool IsSceneActive(const scene::Scene& scene);
	inline std::unordered_map<std::string, scene::Scene*>& GetAllScenes() { return m_Scenes; }

#pragma endregion

#pragma region Light

private:
	LightingManager* m_LightingManager;

public:
	inline LightingManager& GetLightingManager() { return *m_LightingManager; }

#pragma endregion

};