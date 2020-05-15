#include "pch.h"

#include "Batch2D.h"
#include "GL/glew.h"

#include "Engine/Core/Renderer/Renderer.h"

#include "Engine/Core/Vertex.h"
#include "Engine/Core/VertexArray.h"
#include "Engine/Core/Buffers/VertexBuffer.h"
#include "Engine/Core/Buffers/IndexBuffer.h"

#include "Engine/Core/Shaders/Shader.h"
#include "Engine/Core/Textures/Texture2D.h"

#include "Engine/Core/Renderer/Renderer2D.h"

#include "Engine/Model/Quad.h"

namespace GraphX
{
	Batch2D::Batch2D(uint32_t QuadCount)
		: Batch(QuadCount)
	{
		// NOTE: Vertices and indices counts are set according to the quad
		
		const VertexBufferLayout& Layout = VertexBatch2D::VertexLayout();
		m_VAO = CreateScope<VertexArray>();
		m_VBO = CreateScope<VertexBuffer>(m_MaxVerticesCount * sizeof(VertexBatch2D));
		m_VAO->AddVertexBuffer(*m_VBO, Layout);

		m_IBO = CreateScope<IndexBuffer>(m_MaxIndicesCount);
		m_VAO->AddIndexBuffer(*m_IBO);

		m_VertexData = new VertexBatch2D[m_MaxVerticesCount];
	}

	Batch2D::~Batch2D()
	{
		delete[] m_VertexData;
	}

	void Batch2D::BeginBatch()
	{
		m_VertexDataPtr = m_VertexData;
		m_IndicesDataPtr = m_IndicesData;	
	}

	void Batch2D::Flush()
	{
		// No Need to flush if there is no new data in the buffer
		if (m_VertexDataPtr == m_VertexData)
			return;

		Ref<Shader> shader = Renderer::GetShaderLibrary().GetShader("Batch2D");
		shader->Bind();

		// Bind all the textures
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			glBindTextureUnit(i, m_TextureIDs[i]);
		}

		m_VAO->Bind();

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);

		// Maintain stats
		Renderer2D::s_Data->Stats.DrawCalls++;

		m_Offset = 0;
		m_IndexCount = 0;
		m_TextureSlotIndex = 1;
	}

	void Batch2D::AddQuad(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector4& Color)
	{
		AddQuad(Position, Size, GM::Vector3::ZeroVector, Color);
	}

	void Batch2D::AddQuad(const GM::Vector3& Position, const GM::Vector2& Size, const Ref<Texture2D>& Tex, const GM::Vector4& TintColor, float tiling)
	{
		AddQuad(Position, Size, GM::Vector3::ZeroVector, Tex, TintColor, tiling);
	}

	void Batch2D::AddQuad(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector3& Rotation, const GM::Vector4& Color)
	{
		GX_ENGINE_ASSERT(m_VertexDataPtr != nullptr && m_IndicesDataPtr != nullptr, "Batch::Begin() not called before submitting primities");

		if (IsFull())
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureIndex = 0.0f;		// White Texture Index

		AddQuad_Internal(Position, Size, Rotation, Color, 1.0f, textureIndex);
	}

	void Batch2D::AddQuad(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector3& Rotation, const Ref<class Texture2D>& Tex, const GM::Vector4& TintColor, float tiling)
	{
		GX_ENGINE_ASSERT(m_VertexDataPtr != nullptr && m_IndicesDataPtr != nullptr, "Batch::Begin() not called before submitting primities");

		// If index buffer is full or all texture slots are used
		if (IsFull() || m_TextureSlotIndex == Renderer::MaxTextureImageUnits)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		uint32_t TexID = Tex->GetID();
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
			m_TextureIDs[m_TextureSlotIndex++] = Tex->GetID();
		}

		AddQuad_Internal(Position, Size, Rotation, TintColor, tiling, textureIndex);
	}

	void Batch2D::AddQuad_Internal(const GM::Vector3& Position, const GM::Vector2& Size, const GM::Vector3& Rotation, const GM::Vector4& Color, float tiling, float textureIndex)
	{
		GM::Matrix4 transform = GM::ScaleRotationTranslationMatrix({ Size, 1.0f }, Rotation, Position);

		for (int i = 0; i < 4; i++)
		{
			m_VertexDataPtr->Position = transform * Quad::s_QuadVertexPositions[i];
			m_VertexDataPtr->Color = Color;
			m_VertexDataPtr->TexCoords = Quad::s_QuadVertexTexCoords[i] * tiling;
			m_VertexDataPtr->TexIndex = textureIndex;
			m_VertexDataPtr++;
		}

		for (int i = 0; i < 6; i++)
		{
			*m_IndicesDataPtr = m_Offset + Quad::s_QuadIndices[i];
			m_IndicesDataPtr++;
		}

		m_Offset += 4;
		m_IndexCount += 6;

		// Maintain stats
		Renderer2D::s_Data->Stats.QuadCount++;
	}


	bool Batch2D::IsFull() const
	{
		return m_IndexCount >= m_MaxIndicesCount;
	}

	void Batch2D::EndBatch()
	{
		size_t size = (m_VertexDataPtr - m_VertexData) * sizeof(VertexBatch2D);
		m_VBO->SetData(m_VertexData, 0, size);

		m_IBO->SetData(m_IndicesData, m_IndexCount);

		m_VertexDataPtr = nullptr;
		m_IndicesDataPtr = nullptr;
	}

}