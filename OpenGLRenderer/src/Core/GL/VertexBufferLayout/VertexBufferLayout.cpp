#include "VertexBufferLayout.h"
#include "../../Logger.h"

namespace core::gl
{
	void VertexBufferLayout::Bind()
	{
		unsigned int offset = 0;
		for (unsigned int i = 0; i < m_Elements.size(); i++)
		{
			const auto& element = m_Elements[i];
			if (element.isEnabled)
				continue;

			GLLog(glEnableVertexAttribArray(i));
			GLLog(glVertexAttribPointer(i,
				element.count,
				element.type,
				element.normalized,
				m_Stride,
				(const void*)offset));
			offset += element.count * GetSizeOfType(element.type);

			EnableElement(i);
		}
	}

	void VertexBufferLayout::UnBind()
	{
		unsigned int offset = 0;
		for (unsigned int i = 0; i < m_Elements.size(); i++)
		{
			GLLog(glDisableVertexAttribArray(i));
			DisableElement(i);
		}
	}
}