#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

struct RenderData
{
	VertexArray* m_VertexArray = nullptr;
	IndexBuffer* m_IndexBuffer = nullptr;
	Shader* m_Shader = nullptr;
};