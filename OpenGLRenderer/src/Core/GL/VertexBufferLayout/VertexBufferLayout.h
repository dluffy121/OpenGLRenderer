#pragma once

#include <vector>
#include <unordered_map>
#include <GL/OpenGLHelper.h>
#include <Math/Math.h>

namespace core::gl
{
	struct VertexBufferElement
	{
		GLuint id;
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
		bool isEnabled;
	};

	class VertexBufferLayout
	{
	private:
		std::unordered_map<GLuint, VertexBufferElement> m_Elements;
		unsigned int m_Stride;

	public:
		VertexBufferLayout() :
			m_Stride(0)
		{}

		template<typename T>
		void Push(GLuint id, unsigned int count)
		{
			static_assert(falseType<T>::value);
		}

		void Pop(GLuint id)
		{
			auto vbe = m_Elements[id];
			m_Stride -= vbe.count * GetSizeOfType(vbe.type);
			m_Elements.erase(id);
		}

		template<>
		void Push<float>(GLuint id, unsigned int count)
		{
			m_Elements[id] = { id, GL_FLOAT, count, GL_FALSE };
			m_Stride += count * GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<int>(GLuint id, unsigned int count)
		{
			m_Elements[id] = { id, GL_INT, count, GL_FALSE };
			m_Stride += count * GetSizeOfType(GL_INT);
		}

		template<>
		void Push<unsigned int>(GLuint id, unsigned int count)
		{
			m_Elements[id] = { id, GL_UNSIGNED_INT, count, GL_FALSE };
			m_Stride += count * GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void Push<unsigned char>(GLuint id, unsigned int count)
		{
			m_Elements[id] = { id, GL_UNSIGNED_BYTE, count, GL_TRUE };
			m_Stride += count * GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		template<>
		void Push<Vec2>(GLuint id, unsigned int count)
		{
			count *= 2;
			m_Elements[id] = { id, GL_FLOAT, count, GL_TRUE };
			m_Stride += count * GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<Vec3>(GLuint id, unsigned int count)
		{
			count *= 3;
			m_Elements[id] = { id, GL_FLOAT, count, GL_TRUE };
			m_Stride += count * GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<Vec4>(GLuint id, unsigned int count)
		{
			count *= 4;
			m_Elements[id] = { id, GL_FLOAT, count,  GL_TRUE };
			m_Stride += count * GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<Vertex>(GLuint id, unsigned int count)
		{
			while (count--)
			{
				Push<Vec4>(id++, 1);
				Push<Vec4>(id++, 1);
				Push<Vec2>(id++, 1);
				Push<int>(id++, 1);
			}
		}

		inline const std::unordered_map<GLuint, VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
		void Bind();
		void UnBind();
	};
}