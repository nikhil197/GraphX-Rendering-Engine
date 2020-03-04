#pragma once

#include "Batch.h"
#include "Engine/Core/Renderer/Renderer.h"

namespace GraphX
{
	class ParticleBatch
		: public Batch
	{
		friend class Renderer2D;
	public:
		/**
		* Batch2D
		* @size - number of elements / primitives (quads) that the batch can hold
		*/
		ParticleBatch(uint32_t QuadCount);
		~ParticleBatch();

		void BeginBatch() override;
		void EndBatch() override;

		void Flush() override;

		void AddParticle(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector4& Color);
		void AddParticle(const GM::Vector3& Position, const GM::Vector2& Size, const Ref<class Texture2D>& Texture, const GM::Vector4& TexOffsets, float BlendFactor);

		bool IsFull() const override;
	private:
		void AddParticle_Internal(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector4& Color, const GM::Vector4& TexOffsets, float BlendFactor, float TexAtlasRows, float TextureIndex);

	private:
		// Store Texture ID's of the textures used
		std::array<uint32_t, Renderer::MaxTextureImageUnits> m_TextureIDs;

		const size_t m_MaxVerticesCount;
		const size_t m_MaxIndicesCount;

		// Buffer to store vertices
		struct VertexParticleBatch* m_VertexData = nullptr;
		struct VertexParticleBatch* m_VertexDataPtr = nullptr;

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