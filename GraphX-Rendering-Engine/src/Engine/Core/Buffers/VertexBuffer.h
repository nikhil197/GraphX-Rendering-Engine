#pragma once

#include "Engine/Core/RendererResource.h"

namespace GraphX
{
	/* Class to represent the buffers */
	class VertexBuffer
		: public RendererResource
	{
	public:
		/*data is the collection of the vertices and size is the size of the collection in bytes */
		VertexBuffer(const void* data, uint32_t size);
		
		/* Creates an empty vertex buffer of size 'size' (data to be specified later) */
		VertexBuffer(uint32_t size);

		/* Copy construct another buffer */
		VertexBuffer(const VertexBuffer& Other);

		/* Renderer Resource Interface */
		virtual void InitRHI() override;

		virtual void ReleaseRHI() override;

		/** ~ Renderer Resource interface */

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

		/* Returns the total memory Allocated to the Vertex Buffers */
		static inline uint64_t TotalAllocatedMemory() { return s_TotalAllocatedMemory; }

	private:
		/* Total memory allocated to the vertex buffers in the engine */
		static uint64_t s_TotalAllocatedMemory;

		/* Total Size (in bytes) of the data stored in the buffer */
		uint32_t m_BufferSize;
	};
}