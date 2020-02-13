#include "pch.h"
#include "IndexBuffer.h"
#include "GL/glew.h"

namespace GraphX
{
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		:m_Count(count)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(const IndexBuffer& Other)
		: m_Count(Other.m_Count)
	{
		// Bind source buffer
		glBindBuffer(GL_COPY_READ_BUFFER, Other.m_RendererID);

		// Create the new buffer (data not specified)
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

		// Copy data from the source buffer
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, m_Count * sizeof(unsigned int));

		// Unbind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
}