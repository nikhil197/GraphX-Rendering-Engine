#pragma once

namespace GraphX
{
	class VertexBuffer;
	class VertexBufferLayout;
	class IndexBuffer;

	class VertexArray
	{
	public:
		VertexArray();

		~VertexArray();

		/* Add new buffer to the vao object to be bound */
		void AddVertexBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

		void AddIndexBuffer(const IndexBuffer& ibo);

		/* Bind the vao */
		void Bind() const;

		/* UnBind the vao */
		void UnBind() const;
	
	private:
		/* Identifier for the vao */
		unsigned int m_RendererID;
	};
}