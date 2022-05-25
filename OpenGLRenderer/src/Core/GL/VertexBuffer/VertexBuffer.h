#pragma once

namespace core::gl
{
	class VertexBuffer
	{
	private:
		unsigned int m_VertexBufferId;

	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;
	};
}