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

	bool m_initialized;

public:
	static WindowManager* getInstance();

	Window* GetWindowInstance(const std::string& windowId, int width, int height, GLFWwindow* sharedWindow = NULL, ImFontAtlas* sharedFontAtlas = NULL);

	void Init();
	void WindowLoop();
	void Exit();

	inline Window*& GetCurrentWindow() { return m_currentWindow; }

protected:
	WindowManager();

private:
	WindowManager(const WindowManager&) = delete;
	WindowManager operator=(const WindowManager&) = delete;

	void InitImGuiRendererBackend();
	void ShutdownImGuiRendererBackend();
};