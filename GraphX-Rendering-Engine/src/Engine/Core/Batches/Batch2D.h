#pragma once

#include "Batch.h"
#include "Engine/Core/Renderer/Renderer.h"

namespace GraphX
{
	class Batch2D
		: public Batch
	{
		friend class Renderer2D;
	public:
		/**
		* Batch2D
		* @size - number of elements / primitives (quads) that the batch can hold
		*/
		Batch2D(uint32_t QuadCount);
		~Batch2D();

		void BeginBatch() override;
		void EndBatch() override;

		void Flush() override;

		void AddQuad(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector4& Color);
		void AddQuad(const GM::Vector3& Position, const GM::Vector2& Size, const Ref<class Texture2D>& Tex);

		bool IsFull() const override;

	private:
		void AddQuad_Internal(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector4& Color, float textureIndex);

	private:
		// Store Texture ID's of the textures used
		std::array<uint32_t, Renderer::MaxTextureImageUnits> m_TextureIDs;
		
		const size_t m_MaxVerticesCount;
		const size_t m_MaxIndicesCount;
		
		// Buffer to store vertices
		struct VertexBatch2D* m_VertexData = nullptr;
		struct VertexBatch2D* m_VertexDataPtr = nullptr;

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