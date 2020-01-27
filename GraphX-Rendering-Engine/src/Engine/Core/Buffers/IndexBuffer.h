#pragma once

namespace GraphX
{
	class IndexBuffer
	{
	public:
		/* data is the collection of indices and size is the size of collection in bytes */
		IndexBuffer(const unsigned int* data, unsigned int count);

		/* Copy Construct an index buffer */
		IndexBuffer(const IndexBuffer& Other);

		/* Bind the index buffer */
		void Bind() const;

		/* Un Bind the index buffer */
		void UnBind() const;

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