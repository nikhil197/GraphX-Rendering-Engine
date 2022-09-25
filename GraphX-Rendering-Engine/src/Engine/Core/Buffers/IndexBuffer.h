#pragma once

#include "Engine/Core/RendererResource.h"

namespace GraphX
{
	// NOTE: GraphX only support 32-bit index buffers
	class IndexBuffer
		: public RendererResource
	{
	public:
		/* data is the collection of indices and size is the size of collection in bytes */
		IndexBuffer(const uint32_t* data, uint32_t count);

		/* Creates an empty index buffer (data to be specified using SetData() )*/
		IndexBuffer(const uint32_t count);

		/* Copy Construct an index buffer */
		IndexBuffer(const IndexBuffer& Other);

		/* Renderer Resource Interface */
		virtual void InitRHI() override;

		virtual void ReleaseRHI() override;

		/** ~ Renderer Resource interface */

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
		uint32_t GetCount() const { return m_Count; }

		/* Returns the total memory Allocated to the Vertex Buffers */
		static inline uint64_t TotalAllocatedMemory() { return s_TotalAllocatedMemory; }

	private:
		/* Total memory allocated to the vertex buffers in the engine */
		static uint64_t s_TotalAllocatedMemory;
		
		/* # of primitives to be drawn */
		uint32_t m_Count;
	};
}