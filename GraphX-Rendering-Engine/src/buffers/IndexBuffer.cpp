#include "IndexBuffer.h"

#include "../ErrorHandler.h"

namespace engine
{
	namespace buffers
	{
		IndexBuffer::IndexBuffer(const void* data, unsigned int size)
		{
			GLCall(glGenBuffers(1, &m_RendererID));
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		}

		void IndexBuffer::Bind() const
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		}

		void IndexBuffer::UnBind() const
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		IndexBuffer::~IndexBuffer()
		{
			GLCall(glDeleteBuffers(1, &m_RendererID));
		}
	}
}