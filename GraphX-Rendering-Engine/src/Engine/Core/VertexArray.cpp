#include "pch.h"
#include "VertexArray.h"
#include "ErrorHandler.h"

#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"

namespace GraphX
{
	static GLenum BufferDataTypeToOpenGLType(BufferDataType Type)
	{
		switch (Type)
		{
			case BufferDataType::Float:		return GL_FLOAT;
			case BufferDataType::Float2:	return GL_FLOAT;
			case BufferDataType::Float3:	return GL_FLOAT;
			case BufferDataType::Float4:	return GL_FLOAT;
			case BufferDataType::Mat3:		return GL_FLOAT;
			case BufferDataType::Mat4:		return GL_FLOAT;
			case BufferDataType::Int:		return GL_INT;
			case BufferDataType::Int2:		return GL_INT;
			case BufferDataType::Int3:		return GL_INT;
			case BufferDataType::Int4:		return GL_INT;
			case BufferDataType::UInt:		return GL_UNSIGNED_INT;
			case BufferDataType::Bool:		return GL_BOOL;
		}

		GX_ASSERT(false, "Unknown Buffer Data Type");
		return 0;
	}

	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	void VertexArray::AddVertexBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
	{
		// Bind both the vertex array and the buffer before specifying the layout
		GLCall(glBindVertexArray(m_RendererID));
		vbo.Bind();
			
		const auto& elements = layout.GetElements();

		for (unsigned int i = 0; i < elements.size(); i++)
		{
			//Enable the current vertex attribute array
			GLCall(glEnableVertexAttribArray(i));

			//specify the layout
			const auto& element = elements[i];
			GLCall(glVertexAttribPointer(i, element.GetComponentCount(), BufferDataTypeToOpenGLType(element.Type), element.Normalised, layout.GetStride(), (const void*)element.Offset));
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