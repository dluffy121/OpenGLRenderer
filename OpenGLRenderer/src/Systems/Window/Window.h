#pragma once

#include "../Core/GL/OpenGLHelper.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <imgui/imgui.h>
#include <Action.h>
#include "../Core/GL/VertexArray/VertexArray.h"
#include "../Scene/Scene.h"
#include "../../Core/Math.h"
#include "../../Core/GL/VertexBuffer/VertexBuffer.h"
#include "../../Core/GL/IndexBuffer/IndexBuffer.h"
#include "../../Core/GL/VertexBufferLayout/VertexBufferLayout.h"

class GameVastu;
class Component;
class Camera;
class GUIWindow;

using namespace core;
using namespace core::gl;

class Window
{
public:
	GameVastu* SelectedGameVastu;

	const size_t MAX_VERTEX_COUNT = 1020;

	const size_t MAX_TRIANGLE_COUNT = MAX_VERTEX_COUNT / 3;
	const size_t MAX_QUAD_COUNT = MAX_VERTEX_COUNT / 4;

	const size_t MAX_TRI_INDEX_COUNT = MAX_TRIANGLE_COUNT * 3;
	const size_t MAX_QUAD_INDEX_COUNT = MAX_QUAD_COUNT * 6;

private:
	std::string m_WindowId;
	GLFWwindow* m_GLFWWindow;
	ImGuiContext* m_ImGuiContext;

	int m_Width;
	int m_Height;

	unsigned int m_Frames;

	VertexArray* m_VAO;

	Camera* m_Camera;

	std::vector<Component*> m_Components;
	std::vector<GUIWindow*> m_GUIWindows;
	std::unordered_map<std::string, scene::Scene*> m_Scenes;

	bool m_buffersBound;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;
	VertexBufferLayout* m_VBLayout;

	unsigned int m_VertexCount;
	unsigned int m_IndexCount;
	unsigned long long m_VBOffset;
	unsigned long long m_IBOffset;

public:
	Window(const std::string& rendererId, int width = 640, int height = 480, GLFWwindow* sharedWindow = NULL, ImFontAtlas* sharedFontAtlas = NULL);
	~Window();

	void NewFrame();
	void Clear();
	void Update();
	void Render();
	void RenderGUI();

	inline Camera& GetCamera() const { return *m_Camera; }
	inline void SetCamera(Camera* camera) { m_Camera = camera; }

	inline std::string GetWindowId() const { return m_WindowId; }
	inline GLFWwindow* GetGLFWWindow() const { return m_GLFWWindow; }
	inline ImGuiContext* GetImGuiContext() const { return m_ImGuiContext; }
	inline VertexArray& GetVertexArrayObject() const { return *m_VAO; }
	inline int GetWindowWidth() { return m_Width; }
	inline int GetWindowHeight() { return m_Height; }
	inline int GetFrames() { return m_Frames; }

	void RegisterComponent(Component& component);
	void UnRegisterComponent(Component& component);

	void RegisterGUIWindow(GUIWindow& window);
	void UnRegisterGUIWindow(GUIWindow& window);

#pragma region Scene
	void AddScene(scene::Scene* scene);
	void RemoveScene(const std::string& sceneName);
	void RemoveScene(const scene::Scene& scene);
	bool IsSceneActive(const std::string& sceneName);
	bool IsSceneActive(const scene::Scene& scene);
	inline std::unordered_map<std::string, scene::Scene*>& GetAllScenes() { return m_Scenes; }
#pragma endregion

#pragma region Batch Render
	inline VertexBuffer* GetVertexBuffer() { return m_VertexBuffer; }
	inline IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; }
	inline VertexBufferLayout* GetVertexBufferLayout() { return m_VBLayout; }
	void AddBufferData(Vertex*& vertices, unsigned int vCount, unsigned int*& indices, unsigned int iCount);
	void AddVertices(Vertex*& vertices, unsigned int count);
	void AddIndices(unsigned int*& indices, unsigned int count);
	unsigned long long UpdateVertexBufferOffset(unsigned long long size);
	unsigned long long UpdateIndexBufferOffset(unsigned long long size);
	inline unsigned int GetIndexOffset() { return m_IndexCount; }
#pragma endregion


private:
	void CreateImGUIContext(ImFontAtlas* sharedFontAtlas);

	void InstallCallbacks();
	void CursorEnterCallback(GLFWwindow* glfwWindow, int entered);
	void CursorPosCallback(GLFWwindow* glfwWindow, double x, double y);
	void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
	void ScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset);
	void KeyCallback(GLFWwindow* glfwWindow, int keycode, int scancode, int action, int mods);
	void CharCallback(GLFWwindow* glfwWindow, unsigned int c);

	void CheckBuffer(uint8_t vbOffset = 0, uint8_t ibOffset = 0, uint8_t vbSize = 0, uint8_t ibSize = 0) const;
};