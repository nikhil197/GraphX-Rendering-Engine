#include "pch.h"
#include "BufferAllocator.h"

namespace GraphX
{
	std::mutex BufferAllocator::s_VertexBuffersMutex;
	std::unordered_map<std::size_t, Ref<VertexBuffer>> BufferAllocator::s_AllocatedVertexBuffers;

	std::mutex BufferAllocator::s_IndexBuffersMutex;
	std::unordered_map<std::size_t, Ref<IndexBuffer>> BufferAllocator::s_AllocatedIndexBuffers;

	std::atomic<std::uint64_t> BufferAllocator::s_AllocatedBufferMemory;

	Ref<IndexBuffer> BufferAllocator::AllocateIndexBuffer(const std::vector<uint32_t>& Indices)
	{
		std::size_t indicesHash = GetIndicesHash(Indices);
		auto itr = s_AllocatedIndexBuffers.find(indicesHash);
		if (itr != s_AllocatedIndexBuffers.end())
		{
			return itr->second;
		}
		else
		{
			Ref<IndexBuffer> buffer = CreateRef<IndexBuffer>(&Indices[0], Indices.size());

			{
				std::lock_guard<std::mutex> lock(s_IndexBuffersMutex);
				s_AllocatedIndexBuffers[indicesHash] = buffer;
			}

			s_AllocatedBufferMemory += Indices.size() * sizeof(uint32_t);
			return buffer;
		}
	}
}