#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct RenderData
{
	VertexArray* VertexArray = nullptr;
	IndexBuffer* IndexBuffer = nullptr;
	std::vector<Shader*> Shaders;
	std::vector<Texture*> Textures;

	int widthRatio;
	int heightRatio;

	float OrthoMultiplier = 1.0f;

	glm::mat4 Projection;
};