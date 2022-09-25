#pragma once

#include "Batch.h"

namespace GraphX
{
	class ParticleBatch
		: public Batch
	{
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

		void AddParticle(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Rotator& Rotation, const GM::Vector4& Color);
		void AddParticle(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Rotator& Rotation, const Ref<class Texture2D>& Texture, const uint32_t SubTextureIndex1, const uint32_t SubTextureIndex2, const GM::Vector4& TintColor, float BlendFactor);

		bool IsFull() const override;
	private:
		void AddParticle_Internal(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Rotator& Rotation, const GM::Vector4& Color, const GM::Vector2* TextureCoords1, const GM::Vector2* TextureCoords2, float BlendFactor, float TexAtlasRows, float TextureIndex);

	private:
		// Buffer to store vertices
		struct VertexParticleBatch* m_VertexData = nullptr;
		struct VertexParticleBatch* m_VertexDataPtr = nullptr;
	};
}