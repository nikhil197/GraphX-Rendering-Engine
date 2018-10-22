#pragma once

namespace engine
{
	namespace buffers
	{
		class IndexBuffer
		{
		private:
			/* ID of the index buffer */
			unsigned int m_RendererID;

		public:
			/* data is the collection of indices and size is the size of collection in bytes */
			IndexBuffer(const void* data, unsigned int size);

			/* Bind the index buffer */
			void Bind() const;

			/* Un Bind the index buffer */
			void UnBind() const;

			~IndexBuffer();
		};
	}
}