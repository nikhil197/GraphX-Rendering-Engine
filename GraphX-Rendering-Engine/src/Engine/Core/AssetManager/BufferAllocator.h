#pragma once

#include "Utilities/HashUtil.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"

namespace GraphX
{
	/*
	* Allocator for creating buffers without duplication.
	* For creating buffer regardless if it is a duplicate, directly use the Buffer abstractions
	*/
	class BufferAllocator
	{
	public:
		template<typename T>
		static Ref<class VertexBuffer> AllocateVertexBuffer(const std::vector<T>& VertexData)
		{
			std::size_t vertexDataHash = GetVerticesHash(VertexData);
			auto itr = s_AllocatedVertexBuffers.find(vertexDataHash);
			if (itr != s_AllocatedVertexBuffers.end())
			{
				return itr->second;
			}
			else
			{
				Ref<VertexBuffer> buffer = CreateRef<VertexBuffer>(&VertexData[0], VertexData.size() * sizeof(T));

				{
					std::lock_guard<std::mutex> lock(s_VertexBuffersMutex);
					s_AllocatedVertexBuffers[vertexDataHash] = buffer;
				}

				s_AllocatedBufferMemory += VertexData.size() * sizeof(T);
				return buffer;
			}
		}

		/* Returns the existing index buffer else creates a new one */
		static Ref<IndexBuffer> AllocateIndexBuffer(const std::vector<uint32_t>& Indices);

		/* Returns the total allocated buffer memory */
		static inline std::uint64_t GetAllocatedBufferMemory() { return s_AllocatedBufferMemory; }

	private:
		/* Sum total of all the memroy allocated to the buffers using buffer allocator */
		static std::atomic<std::uint64_t> s_AllocatedBufferMemory;

		/* Mutex for adding new vertex buffer */
		static std::mutex s_VertexBuffersMutex;

		/* Cache to store the allocated Vertex Buffers */
		static std::unordered_map<std::size_t, Ref<VertexBuffer>> s_AllocatedVertexBuffers;

		/* Mutex for adding new Index Buffer */
		static std::mutex s_IndexBuffersMutex;

		/* Cache to store the allocated Index Buffers */
		static std::unordered_map<std::size_t, Ref<IndexBuffer>> s_AllocatedIndexBuffers;
	};
}