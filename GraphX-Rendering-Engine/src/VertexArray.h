#pragma once

namespace engine
{
	class VertexBuffer;
	class VertexBufferLayout;
	class IndexBuffer;

	class VertexArray
	{
	private:
		/* Identifier for the vao */
		unsigned int m_RendererID;

	public:
		VertexArray();

		~VertexArray();

		/* Add new buffer to the vao object to be bound */
		void AddBuffer(VertexBuffer& vbo, VertexBufferLayout& layout);

		void AddIndexBuffer(const IndexBuffer& ibo);

		/* Bind the vao */
		void Bind() const;

		/* UnBind the vao */
		void UnBind() const;
	};
}