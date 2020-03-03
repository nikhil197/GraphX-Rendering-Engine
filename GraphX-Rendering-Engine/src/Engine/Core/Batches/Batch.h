#pragma once

namespace GraphX
{
	class Batch
	{
	public:
		Batch(uint32_t PrimCount)
			: m_PrimitivesCount(PrimCount)
		{}

		virtual ~Batch() = default;

		virtual void BeginBatch() = 0;
		virtual void EndBatch() = 0;

		virtual void Flush() = 0;
		
		virtual bool IsFull() const = 0;

	protected:
		// Number of primitives the batch can hold
		uint32_t m_PrimitivesCount;
		Scope<class VertexArray> m_VAO;
		Scope<class VertexBuffer> m_VBO;
		Scope<class IndexBuffer> m_IBO;
	};
}