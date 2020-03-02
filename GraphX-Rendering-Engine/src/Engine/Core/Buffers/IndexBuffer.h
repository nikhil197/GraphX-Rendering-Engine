#pragma once

namespace GraphX
{
	class IndexBuffer
	{
	public:
		/* data is the collection of indices and size is the size of collection in bytes */
		IndexBuffer(const unsigned int* data, unsigned int count);

		/* Creates an empty index buffer (data to be specified using SetData() )*/
		IndexBuffer(const uint32_t count);

		/* Copy Construct an index buffer */
		IndexBuffer(const IndexBuffer& Other);

		/* Bind the index buffer */
		void Bind() const;

		/* Un Bind the index buffer */
		void UnBind() const;

		/* Set new data for the index buffer */
		void SetData(const uint32_t* data, uint32_t count);

		/* Sets new data for the buffer over custom range
		* Offset (number of indices) - where in buffer to start
		* count (number of indices) - number of indices in 'data'
		*/
		void SetData(const uint32_t* data, uint32_t offset, uint32_t count);

		~IndexBuffer();

		/* Returns the count */
		unsigned int GetCount() const { return m_Count; }

	private:
		/* ID of the index buffer */
		unsigned int m_RendererID;

		/* # of primitives to be drawn */
		unsigned int m_Count;
	};
}