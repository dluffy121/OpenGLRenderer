#include "VertexBufferLayout.h"
#include <Logger/Logger.h>
#include <Window/WindowManager.h>

namespace core::gl
{
	void VertexBufferLayout::Bind()
	{
		unsigned int offset = 0;
		unsigned int index = 0;
		for (const auto& vbElement : m_Elements)
		{
			const auto& element = vbElement.second;
			if (element.isEnabled)
				continue;

			GLLog(glEnableVertexAttribArray(index));
			GLLog(glVertexAttribPointer(index,
				element.count,
				element.type,
				element.normalized,
				m_Stride,
				(const void*)offset));
			offset += element.count * GetSizeOfType(element.type);
			index++;
		}
	}

	void VertexBufferLayout::UnBind()
	{
		unsigned int offset = 0;
		unsigned int index = 0;
		for (const auto& vbElement : m_Elements)
		{
			GLLog(glDisableVertexAttribArray(index++));
		}
	}
}