#pragma once

#include "Batch.h"

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
		// Buffer to store vertices
		struct VertexBatch2D* m_VertexData = nullptr;
		struct VertexBatch2D* m_VertexDataPtr = nullptr;
	};
}