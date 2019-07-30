#include "pch.h"
#include "VertexArray.h"

#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"

namespace engine
{
	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	void VertexArray::AddBuffer(VertexBuffer& vbo, VertexBufferLayout& layout)
	{
		// Bind both the vertex array and the buffer before specifying the layout
		GLCall(glBindVertexArray(m_RendererID));
		vbo.Bind();
			
		const auto& elements = layout.GetElements();

		// Offset of the current vertex attribute (zero for first attribute)
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			//Enable the current vertex attribute array
			GLCall(glEnableVertexAttribArray(i));

			//specify the layout
			const auto& element = elements[i];
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)offset));
				
			//increase the offset
			offset += element.count * BufferLayoutElement::GetSizeOfType(element.type);
		}

		// Unbind the vertex array
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::AddIndexBuffer(const IndexBuffer& IBO)
	{
		GLCall(glBindVertexArray(m_RendererID));
		IBO.Bind();
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::UnBind() const
	{
		GLCall(glBindVertexArray(0));
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}
}