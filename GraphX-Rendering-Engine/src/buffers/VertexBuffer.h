#pragma once

namespace GraphX
{
	/* Class to represent the buffers */
	class VertexBuffer
	{
	private:
		/* ID for the buffer */
		unsigned int m_RendererID;

	public:
		/*data is the collection of the vertices and size is the size of the collection in bytes */
		VertexBuffer(const void* data, unsigned int size);
			
		/* Bind the buffer to the target */
		void Bind() const;

		/* Un Bind the buffer for the target */
		void UnBind() const;

		~VertexBuffer();
	};
}