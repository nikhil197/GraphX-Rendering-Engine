#include "pch.h"
#include "VertexBuffer.h"

namespace GraphX
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
		: m_BufferSize(size)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_BufferSize, data, GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	VertexBuffer::VertexBuffer(const VertexBuffer& Other)
		: m_BufferSize(Other.m_BufferSize)
	{
		// Bind source buffer
		GLCall(glBindBuffer(GL_COPY_READ_BUFFER, Other.m_RendererID));

		// Create the new buffer (data not specified)
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_BufferSize, nullptr, GL_STATIC_DRAW));

		// Copy data from the source buffer
		GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_BufferSize));

		// Unbind
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_COPY_READ_BUFFER, 0));
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