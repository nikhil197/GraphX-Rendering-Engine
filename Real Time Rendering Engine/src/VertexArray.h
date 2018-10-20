#pragma once

#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"

namespace engine
{
	namespace arrays
	{
		class VertexArray
		{
		private:
			/* Identifier for the vao */
			unsigned int m_RendererID;

		public:
			VertexArray();

			~VertexArray();

			/* Add new buffer to the vao object to be bound */
			void AddBuffer(buffers::VertexBuffer& vbo, buffers::VertexBufferLayout& layout);

			/* Bind the vao */
			void Bind() const;

			/* Un Bind the vao */
			void UnBind() const;
		};
	}
}