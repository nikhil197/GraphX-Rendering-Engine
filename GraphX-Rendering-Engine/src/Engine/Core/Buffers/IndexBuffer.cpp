#include "pch.h"
#include "IndexBuffer.h"
#include "GL/glew.h"

namespace GraphX
{
	uint64_t IndexBuffer::s_TotalAllocatedMemory;

	IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
		: RendererResource(), m_Count(count)
	{
		GX_PROFILE_FUNCTION()

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), data, GL_STATIC_DRAW);

		s_TotalAllocatedMemory += m_Count * sizeof(uint32_t);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(const uint32_t count)
		: RendererResource(), m_Count(count)
	{
		GX_PROFILE_FUNCTION()

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(const IndexBuffer& Other)
		: RendererResource(), m_Count(Other.m_Count)
	{
		GX_PROFILE_FUNCTION()

		// Bind source buffer
		glBindBuffer(GL_COPY_READ_BUFFER, Other.m_RendererID);

		// Create the new buffer (data not specified)
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

		// Copy data from the source buffer
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, m_Count * sizeof(unsigned int));

		s_TotalAllocatedMemory += m_Count * sizeof(uint32_t);

		// Unbind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
	}

	void IndexBuffer::InitRHI()
	{

	}

	void IndexBuffer::ReleaseRHI()
	{

	}

	void IndexBuffer::Bind() const
	{
		GX_PROFILE_FUNCTION()

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::UnBind() const
	{
		GX_PROFILE_FUNCTION()

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetData(const uint32_t* data, uint32_t count)
	{
		SetData(data, 0, count);
	}

	void IndexBuffer::SetData(const uint32_t* data, uint32_t offset, uint32_t count)
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(count - offset <= m_Count, "Data not provided properly");

		glBindVertexArray(0);	// To make sure buffer doesn't get bound to other vaos'

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(uint32_t), count * sizeof(uint32_t), data);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::~IndexBuffer()
	{
		GX_PROFILE_FUNCTION()

		glDeleteBuffers(1, &m_RendererID);
	}
}