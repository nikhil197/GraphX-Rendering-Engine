#pragma once

#include "Engine/Core/RendererResource.h"

namespace GraphX
{
	class VertexBuffer;
	class VertexBufferLayout;
	class IndexBuffer;

	class VertexArray
		: public RendererResource
	{
	public:
		VertexArray();

		~VertexArray();

		/* Add new buffer to the vao object to be bound */
		void AddVertexBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

		void AddIndexBuffer(const IndexBuffer& ibo);

		/* Renderer Resource Interface */
		virtual void InitRHI() override;

		virtual void ReleaseRHI() override;

		/** ~ Renderer Resource interface */

		/* Bind the vao */
		void Bind() const;

		/* UnBind the vao */
		void UnBind() const;

	private:
		uint32_t m_ActiveVertexAttribs = 0;
	};
}