#pragma once

#include "Batch.h"

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

		void AddParticle(const GM::Vector3& Position, const GM::Vector2& Size, float Rotation, const GM::Vector4& Color);
		void AddParticle(const GM::Vector3& Position, const GM::Vector2& Size, float Rotation, const Ref<class Texture2D>& Texture, const GM::Vector4& TintColor, const GM::Vector4& TexOffsets, float BlendFactor);

		bool IsFull() const override;
	private:
		void AddParticle_Internal(const GM::Vector3& Position, const GM::Vector2& Size, float Rotation, const GM::Vector4& Color, const GM::Vector4& TexOffsets, float BlendFactor, float TexAtlasRows, float TextureIndex);

	private:
		// Buffer to store vertices
		struct VertexParticleBatch* m_VertexData = nullptr;
		struct VertexParticleBatch* m_VertexDataPtr = nullptr;
	};
}