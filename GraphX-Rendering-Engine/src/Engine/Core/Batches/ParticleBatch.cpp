#include "pch.h"

#include "ParticleBatch.h"

#include "GL/glew.h"

#include "Engine/Core/Vertex.h"
#include "Engine/Core/VertexArray.h"

#include "Engine/Core/Buffers/VertexBuffer.h"
#include "Engine/Core/Buffers/IndexBuffer.h"
#include "Engine/Core/Buffers/VertexBufferLayout.h"

#include "Engine/Core/Shaders/Shader.h"
#include "Engine/Core/Textures/Texture2D.h"

namespace GraphX
{
	ParticleBatch::ParticleBatch(uint32_t QuadCount)
		: Batch(QuadCount), m_MaxVerticesCount(4 * m_PrimitivesCount), m_MaxIndicesCount(6 * m_PrimitivesCount)
	{
		const VertexBufferLayout& Layout = VertexParticleBatch::VertexLayout();
		m_VAO = CreateScope<VertexArray>();
		m_VBO = CreateScope<VertexBuffer>(m_MaxVerticesCount * sizeof(VertexParticleBatch));
		m_VAO->AddVertexBuffer(*m_VBO, Layout);

		m_IBO = CreateScope<IndexBuffer>(m_MaxIndicesCount);
		m_VAO->AddIndexBuffer(*m_IBO);

		m_VertexData = new VertexParticleBatch[m_MaxVerticesCount];
		m_IndicesData = new uint32_t[m_MaxIndicesCount];

		// TODO: Replace 32, get the actual count from the GPU
		for (int i = 0; i < Renderer::MaxTextureImageUnits; i++)
		{
			m_TextureIDs[i] = 0;
		}
	}

	ParticleBatch::~ParticleBatch()
	{
		delete[] m_VertexData;
		delete[] m_IndicesData;
	}

	void ParticleBatch::BeginBatch()
	{
		m_VertexDataPtr = m_VertexData;
		m_IndicesDataPtr = m_IndicesData;
	}

	void ParticleBatch::EndBatch()
	{
		size_t size = (m_VertexDataPtr - m_VertexData) * sizeof(VertexParticleBatch);
		m_VBO->SetData(m_VertexData, 0, size);

		m_IBO->SetData(m_IndicesData, m_IndexCount);

		m_VertexDataPtr = nullptr;
		m_IndicesDataPtr = nullptr;
	}

	void ParticleBatch::Flush()
	{
		// No Need to flush if there is no new data in the buffer
		if (m_VertexDataPtr == m_VertexData)
			return;

		Ref<Shader> shader = Renderer::GetShaderLibrary().GetShader("ParticleBatch");
		shader->Bind();

		// Pre Render stuff
		glDepthMask(false);		// Don't render the particles to the depth buffer

		glEnable(GL_BLEND);		// To enable blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		// Bind all the textures
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			glBindTextureUnit(i, m_TextureIDs[i]);
		}

		m_VAO->Bind();

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);

		// Post Render Stuff
		glDepthMask(true);
		glDisable(GL_BLEND);

		m_Offset = 0;
		m_IndexCount = 0;
		m_TextureSlotIndex = 1;
	}

	void ParticleBatch::AddParticle(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector4& Color)
	{
		GX_ENGINE_ASSERT(m_VertexDataPtr != nullptr && m_IndicesDataPtr != nullptr, "Batch::Begin() not called before submitting primities");

		if (IsFull())
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = 0.0f;		// White Texture Index

		AddParticle_Internal(Position, Size, Color, GM::Vector4::ZeroVector, 0.0f, 0, textureIndex);
	}

	void ParticleBatch::AddParticle(const GM::Vector3& Position, const GM::Vector2& Size, const Ref<Texture2D>& Texture, const GM::Vector4& TexOffsets, float BlendFactor)
	{
		GX_ENGINE_ASSERT(m_VertexDataPtr != nullptr && m_IndicesDataPtr != nullptr, "Batch::Begin() not called before submitting primities");

		// If index buffer is full or all texture slots are used
		if (IsFull() || m_TextureSlotIndex == Renderer::MaxTextureImageUnits)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		uint32_t TexID = Texture->GetID();
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < m_TextureSlotIndex; i++)
		{
			if (TexID == m_TextureIDs[i])
			{
				textureIndex = (float)i;
				break;
			}
		}

		// If texture is not in the texture slots
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)m_TextureSlotIndex;
			m_TextureIDs[m_TextureSlotIndex++] = Texture->GetID();
		}

		AddParticle_Internal(Position, Size, GM::Vector4::UnitVector, TexOffsets, BlendFactor, (float)Texture->GetRowsInTexAtlas(), textureIndex);
	}

	void ParticleBatch::AddParticle_Internal(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector4& Color, const GM::Vector4& TexOffsets, float BlendFactor, float TexAtlasRows, float TextureIndex)
	{
		m_VertexDataPtr->Position = { Position.x, Position.y, Position.z };
		m_VertexDataPtr->Color = Color;
		m_VertexDataPtr->TexCoords = { 0.0f, 0.0f };
		m_VertexDataPtr->TexOffsets = TexOffsets;
		m_VertexDataPtr->TexAtlasRows = TexAtlasRows;
		m_VertexDataPtr->BlendFactor = BlendFactor;
		m_VertexDataPtr->TexIndex = TextureIndex;
		m_VertexDataPtr++;

		m_VertexDataPtr->Position = { Position.x + Size.x, Position.y, Position.z };
		m_VertexDataPtr->Color = Color;
		m_VertexDataPtr->TexCoords = { 1.0f, 0.0f };
		m_VertexDataPtr->TexIndex = TextureIndex;
		m_VertexDataPtr->TexOffsets = TexOffsets;
		m_VertexDataPtr->TexAtlasRows = TexAtlasRows;
		m_VertexDataPtr->BlendFactor = BlendFactor;
		m_VertexDataPtr->TexIndex = TextureIndex;
		m_VertexDataPtr++;

		m_VertexDataPtr->Position = { Position.x + Size.x, Position.y + Size.y, Position.z };
		m_VertexDataPtr->Color = Color;
		m_VertexDataPtr->TexCoords = { 1.0f, 1.0f };
		m_VertexDataPtr->TexOffsets = TexOffsets;
		m_VertexDataPtr->TexAtlasRows = TexAtlasRows;
		m_VertexDataPtr->BlendFactor = BlendFactor;
		m_VertexDataPtr->TexIndex = TextureIndex;
		m_VertexDataPtr++;

		m_VertexDataPtr->Position = { Position.x, Position.y + Size.y, Position.z };
		m_VertexDataPtr->Color = Color;
		m_VertexDataPtr->TexCoords = { 0.0f, 1.0f };
		m_VertexDataPtr->TexOffsets = TexOffsets;
		m_VertexDataPtr->TexAtlasRows = TexAtlasRows;
		m_VertexDataPtr->BlendFactor = BlendFactor;
		m_VertexDataPtr->TexIndex = TextureIndex;
		m_VertexDataPtr++;

		*m_IndicesDataPtr = 0 + m_Offset; m_IndicesDataPtr++;
		*m_IndicesDataPtr = 1 + m_Offset; m_IndicesDataPtr++;
		*m_IndicesDataPtr = 2 + m_Offset; m_IndicesDataPtr++;
		*m_IndicesDataPtr = 2 + m_Offset; m_IndicesDataPtr++;
		*m_IndicesDataPtr = 3 + m_Offset; m_IndicesDataPtr++;
		*m_IndicesDataPtr = 0 + m_Offset; m_IndicesDataPtr++;
		m_Offset += 4;
		m_IndexCount += 6;
	}

	bool ParticleBatch::IsFull() const
	{
		return m_IndexCount >= m_MaxIndicesCount;
	}
}