#pragma once

#include "RenderData.h"
#include <functional>
#include <GLFW/glfw3.h>
#include "Action.h"
#include "Camera.h"
#include "Renderer.h"
#include "GameVastu.h"

class Window
{
public:
	std::vector<Action> UpdateActions;

	glm::mat4 m_Projection;

private:
	std::string m_WindowId;
	GLFWwindow* m_GLFWWindow;

	int m_Width;
	int m_Height;

	VertexArray* m_VA;

	Camera* m_Camera;

	std::vector<const GameVastu*> m_GameVastus;
	std::vector<const Renderer*> m_Renderers;

public:
	Window(const std::string& rendererId, int width = 640, int height = 480, GLFWwindow* sharedWindow = NULL);
	~Window();

	void Clear() const;
	void Draw() const;

	inline Camera& GetCamera() const { return *m_Camera; }
	inline void SetCamera(Camera& camera) { m_Camera = &camera; }

	inline std::string GetWindowId() const { return m_WindowId; }
	inline GLFWwindow*& GetGLFWWindow() { return m_GLFWWindow; }
	inline int GetWindowWidth() { return m_Width; }
	inline int GetWindowHeight() { return m_Height; }

	void RegisterGameVastu(const GameVastu& gameVastu);
	void UnRegisterGameVastu(const GameVastu& gameVastu);
};