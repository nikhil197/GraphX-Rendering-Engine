#include "pch.h"
#include "GL/glew.h"
#include "VertexBuffer.h"

namespace GraphX
{
	VertexBuffer::VertexBuffer(const void* data, uint32_t size)
		: m_BufferSize(size)
	{
		GX_PROFILE_FUNCTION()

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, m_BufferSize, data, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::VertexBuffer(uint32_t size)
		: m_BufferSize(size)
	{
		GX_PROFILE_FUNCTION()

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, m_BufferSize, nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::VertexBuffer(const VertexBuffer& Other)
		: m_BufferSize(Other.m_BufferSize)
	{
		GX_PROFILE_FUNCTION()

		// Bind source buffer
		glBindBuffer(GL_COPY_READ_BUFFER, Other.m_RendererID);

		// Create the new buffer (data not specified)
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, m_BufferSize, nullptr, GL_STATIC_DRAW);

		// Copy data from the source buffer
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_BufferSize);

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
	}

	void VertexBuffer::Bind() const
	{
		GX_PROFILE_FUNCTION()

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::UnBind() const
	{
		GX_PROFILE_FUNCTION()

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		SetData(data, 0, size);
	}

	void VertexBuffer::SetData(const void* data, size_t offset, size_t size)
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(size - offset <= m_BufferSize, "Data not provided properly")

		glBindVertexArray(0);	// To make sure buffer doesn't get bound to other vaos'

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::~VertexBuffer()
	{
		GX_PROFILE_FUNCTION()

		glDeleteBuffers(1, &m_RendererID);
	}
}