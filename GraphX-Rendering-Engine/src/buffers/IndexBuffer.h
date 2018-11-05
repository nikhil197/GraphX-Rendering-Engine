#pragma once

namespace engine
{
	class IndexBuffer
	{
	private:
		/* ID of the index buffer */
		unsigned int m_RendererID;

		/* # of primitives to be drawn */
		unsigned int m_Count;
	public:
		/* data is the collection of indices and size is the size of collection in bytes */
		IndexBuffer(const unsigned int* data, unsigned int count);

		/* Bind the index buffer */
		void Bind() const;

		/* Un Bind the index buffer */
		void UnBind() const;

		~IndexBuffer();

		/* Returns the count */
		unsigned int GetCount() const { return m_Count; }
	};
}