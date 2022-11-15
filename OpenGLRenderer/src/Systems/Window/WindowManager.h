#pragma once

#include "Window.h"
#include <unordered_map>

class WindowManager
{
private:
	int rendererCount;
	std::vector<Window*> m_WindowCollection;

	Window* m_currentWindow;

	bool m_initialized;

public:
	static WindowManager* getInstance();

	Window* GetWindowInstance(const std::string& windowId, int width = 0, int height = 0, GLFWwindow* sharedWindow = NULL, ImFontAtlas* sharedFontAtlas = NULL);

	void Init();
	void WindowLoop();
	void Exit();

	void SelectedGameVastu(GameVastu* gameVastu);
	GameVastu* SelectedGameVastu();

	inline Window*& GetCurrentWindow() { return m_currentWindow; }

protected:
	WindowManager();

private:
	WindowManager(const WindowManager&) = delete;
	WindowManager operator=(const WindowManager&) = delete;

	void InitImGuiRendererBackend();
	void ShutdownImGuiRendererBackend();
};

static Window& CurrentWindow() { return *WindowManager::getInstance()->GetCurrentWindow(); }