#include "VertexBuffer.h"
#include "../ErrorHandler.h"

namespace engine
{
	namespace buffers
	{
		VertexBuffer::VertexBuffer(const void* data, unsigned int size)
		{
			GLCall(glGenBuffers(1, &m_RendererID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		}

		void VertexBuffer::Bind() const
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		}

		void VertexBuffer::UnBind() const
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		VertexBuffer::~VertexBuffer()
		{
			GLCall(glDeleteBuffers(1, &m_RendererID));
		}

	}
}