#pragma once

#include "RenderData.h"
#include <functional>
#include <GLFW/glfw3.h>
#include "Action.h"

class Renderer
{
public:
	RenderData m_RenderData;
	std::vector<Action> UpdateActions;

private:
	std::string m_RendererId;
	GLFWwindow* m_Window;
	bool m_isDirty;

public:
	Renderer(const std::string& rendererId, int width = 640, int height = 480, GLFWwindow* sharedWindow = NULL);
	~Renderer();

	void Clear() const;
	void Draw() const;

	inline std::string GetRendererId() const { return m_RendererId; }
	inline GLFWwindow*& GetRendererWindow() { return m_Window; }

	inline VertexArray& GetVertexArray() const { return *m_RenderData.VertexArray; }
	inline IndexBuffer& GetIndexBuffer() const { return *m_RenderData.IndexBuffer; }
	inline std::vector<Shader*> GetShaders() const { return m_RenderData.Shaders; }
	inline std::vector<Texture*> GetTextures() const { return m_RenderData.Textures; }

	inline void SetVertexArray(VertexArray& va) { m_RenderData.VertexArray = &va; }
	inline void SetIndexBuffer(IndexBuffer& ib) { m_RenderData.IndexBuffer = &ib; }
	inline void AddShader(Shader& shader) { m_RenderData.Shaders.push_back(&shader); }
	inline void AddTexture(Texture& texture) { m_RenderData.Textures.push_back(&texture); }

	void SetOrthoMultiplier(float multiplier);

private:
	void UpdateProjectionMatrix();
};