#pragma once

#include "Window.h"
#include <unordered_map>

class WindowManager
{
public:
	std::vector<Action> UpdateActions;
	std::vector<std::function<void(const Window&)>> UpdateActionsRenderer;

private:
	int rendererCount;
	std::vector<Window*> m_WindowCollection;
	std::vector<GLFWwindow*> m_GLFWWindowCollection;
	std::vector<ImGuiContext*> m_ImGuiContextCollection;

public:
	static WindowManager* getInstance();

	Window* GetWindowInstance(const std::string& windowId, int width, int height, GLFWwindow* sharedWindow = NULL, ImFontAtlas* sharedFontAtlas = NULL);

	void RenderLoop();

protected:
	WindowManager();

private:
	WindowManager(const WindowManager&) = delete;
	WindowManager operator=(const WindowManager&) = delete;
};