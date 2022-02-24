#pragma once

#include "RenderData.h"
#include <functional>
#include <GLFW/glfw3.h>
#include "TypeDefs.h"

class Renderer
{
public:
	RenderData m_RenderData;
	std::vector<Action> UpdateActions;

private:
	std::string m_RendererId;
	GLFWwindow* m_Window;

public:
	Renderer(const std::string& rendererId, int width = 640, int height = 480, GLFWwindow* sharedWindow = NULL);
	~Renderer();

	void Clear() const;
	void Draw() const;

	inline std::string GetRendererId() const { return m_RendererId; }
	inline GLFWwindow*& GetRendererWindow() { return m_Window; }

	inline VertexArray& GetVertexArray() { return *m_RenderData.m_VertexArray; }
	inline IndexBuffer& GetIndexBuffer() { return *m_RenderData.m_IndexBuffer; }
	inline Shader& GetShader() { return *m_RenderData.m_Shader; }

	inline void SetVertexArray(VertexArray& va) { m_RenderData.m_VertexArray = &va; }
	inline void SetIndexBuffer(IndexBuffer& ib) { m_RenderData.m_IndexBuffer = &ib; }
	inline void SetShader(Shader& shader) { m_RenderData.m_Shader = &shader; }
};