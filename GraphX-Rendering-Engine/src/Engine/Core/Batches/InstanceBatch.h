#pragma once

#include "Batch.h"

namespace GraphX
{
	class VertexBuffer;
	class IndexBuffer;

	// For now, this Instanced Batch is assuming that all the instances used the same texture (which obviously won't be the case)
	class InstanceBatch
		: public Batch
	{
	public:
		InstanceBatch(std::size_t BatchHash, uint32_t InstanceCount, const Ref<VertexBuffer>& vData, const struct VertexBufferLayout& bufferLayout, const Ref<const IndexBuffer>& iData);

		void AddMesh(const Ref<Mesh3D>& Mesh);

		// Batch interface

		virtual void BeginBatch() override;
		virtual void EndBatch() override;

		virtual void Flush() override;

		virtual bool IsFull() const override { return false; }

		// END Batch interface

		// TODO: Need to account for the increasing instances
		//void Resize(uint32_t newSize);

		/* Returns the Batch Hash */
		inline std::size_t GetBatchHash() const { return m_BatchHash; }

	private:
		void AddMesh_Internal(const Ref<Mesh3D>& Mesh, const std::vector<float>& texIds);

	protected:
		Ref<VertexBuffer> m_VertexData;
		Ref<IndexBuffer> m_IndexData;

	private:
		/* Hash value of the mesh / model which is instanced with this batch */
		std::size_t m_BatchHash;

		/* Instance data for all the instances in the batch */
		struct PerInstanceData* m_InstanceData;
		struct PerInstanceData* m_InstanceDataPtr;

		// Index at which the next texture will be stored
		uint32_t m_TextureSlotIndex = 1;

		/* Number of instances added to the batch */
		uint32_t m_CurrentInstanceCount;
	};
}