#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

struct RenderData
{
	VertexArray* VertexArray = nullptr;
	IndexBuffer* IndexBuffer = nullptr;
	std::vector<Shader*> Shaders;
	std::vector<Texture*> Textures;
};