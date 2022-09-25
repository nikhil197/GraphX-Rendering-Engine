#pragma once

#include "Engine/Core/Renderer/Renderer.h"

#include "Textures/Texture2D.h"

namespace GraphX
{
	class Batch
	{
	protected:
		Batch(uint32_t PrimCount, bool instanceBatch = false)
			: m_PrimitivesCount(PrimCount), m_MaxVerticesCount(4 * m_PrimitivesCount), m_MaxIndicesCount(6 * m_PrimitivesCount)
		{
			if (!instanceBatch)
			{
				m_IndicesData = new uint32_t[m_MaxIndicesCount];
			}

			// TODO: Replace 32, get the actual count from the GPU
			memset(m_TextureIDs.data(), 0, m_TextureIDs.size() * sizeof(uint32_t));

			// 0 Texture slot is reserved for the white texture
			m_TextureIDs[0] = Renderer::GetTextureLibrary().GetTexture2D("White")->GetID();
		}

	public:

		virtual ~Batch()
		{
			delete[] m_IndicesData;
		}

		virtual void BeginBatch() = 0;
		virtual void EndBatch() = 0;

		virtual void Flush() = 0;
		
		virtual bool IsFull() const = 0;

	protected:
		// Number of primitives the batch can hold
		const uint32_t m_PrimitivesCount;
		const uint32_t m_MaxVerticesCount;
		const uint32_t m_MaxIndicesCount;

		// Store Texture ID's of the textures used
		std::array<uint32_t, Renderer::MaxTextureImageUnits> m_TextureIDs;

		Scope<class VertexArray> m_VAO;
		Scope<class VertexBuffer> m_VBO;
		Scope<class IndexBuffer> m_IBO;

		// Buffer to store Indices
		uint32_t* m_IndicesData = nullptr;
		uint32_t* m_IndicesDataPtr = nullptr;

		// Indices Buffer Utilities
		uint32_t m_IndexCount = 0;
		uint32_t m_Offset = 0;

		// Index at which the next texture will be stored
		uint32_t m_TextureSlotIndex = 1;
	};
}