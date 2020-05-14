#pragma once

namespace GraphX
{
	/* Class to represent the buffers */
	class VertexBuffer
	{
	public:
		/*data is the collection of the vertices and size is the size of the collection in bytes */
		VertexBuffer(const void* data, uint32_t size);
		
		/* Creates an empty vertex buffer of size 'size' (data to be specified later) */
		VertexBuffer(uint32_t size);

		/* Copy construct another buffer */
		VertexBuffer(const VertexBuffer& Other);

		/* Bind the buffer to the target */
		void Bind() const;

		/* Un Bind the buffer for the target */
		void UnBind() const;

		/* Sets new data for the vertex buffer */
		void SetData(const void* data, uint32_t size);

		/* Sets new data for the buffer over custom range
		* Offset (in bytes) - where in buffer to start
		* size (in bytes) - size of 'data' in bytes
		*/
		void SetData(const void* data, size_t offset, size_t size);

		~VertexBuffer();

	private:
		/* ID for the buffer */
		uint32_t m_RendererID;

		/* Total Size (in bytes) of the data stored in the buffer */
		uint32_t m_BufferSize;
	};
}