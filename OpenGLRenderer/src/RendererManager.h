#pragma once

#include "Renderer.h"
#include <unordered_map>

class RendererManager
{
public:
	std::vector<Action> UpdateActions;

private:
	int rendererCount;
	std::vector<Renderer*> m_RendererCollection;
	std::vector<GLFWwindow*> m_WindowCollection;

public:
	static RendererManager* getInstance();

	Renderer* GetRendererInstance(const std::string& rendererId, int width, int height, GLFWwindow* sharedWindow = NULL);

	void RenderLoop();

protected:
	RendererManager();

private:
	RendererManager(const RendererManager&) = delete;
	RendererManager operator=(const RendererManager&) = delete;
};