#pragma once

#include "Window.h"
#include <unordered_map>

class WindowManager
{
public:
	std::vector<Action> UpdateActions;
	std::vector<std::function<void(const Window&)>> UpdateActionsRenderGUI;

private:
	int rendererCount;
	std::vector<Window*> m_WindowCollection;

	Window* m_currentWindow;

	bool m_imGuiRendererInitialized;

public:
	static WindowManager* getInstance();

	Window* GetWindowInstance(const std::string& windowId, int width, int height, GLFWwindow* sharedWindow = NULL, ImFontAtlas* sharedFontAtlas = NULL);

	void WindowLoop();

	inline Window*& GetCurrentWindow() { return m_currentWindow; }

	void InitImGuiRenderer();
	void ShutdownImGuiRenderer();

protected:
	WindowManager();

private:
	WindowManager(const WindowManager&) = delete;
	WindowManager operator=(const WindowManager&) = delete;
};