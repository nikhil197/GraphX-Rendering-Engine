#include "pch.h"
#include "InstanceBatch.h"

#include "Vertex.h"
#include "VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"

#include "Textures/Texture2D.h"

#include "Materials/Material.h"
#include "Shaders/Shader.h"

#include "Model/Mesh/Mesh3D.h"

#include "Renderer/Renderer3D.h"

#include "GL/glew.h"	// For now this is included here, but this should be included at only one place

namespace GraphX
{
	InstanceBatch::InstanceBatch(std::size_t BatchHash, uint32_t InstanceCount, const Ref<VertexBuffer>& vData, const VertexBufferLayout& bufferLayout, const Ref<const IndexBuffer>& iData)
		: Batch(InstanceCount, true), m_BatchHash(BatchHash), m_CurrentInstanceCount(0)
	{
		m_VAO = CreateScope<VertexArray>();

		/* Create the empty buffer */
		m_VBO = CreateScope<VertexBuffer>(InstanceCount * sizeof(PerInstanceData));

		m_VAO->AddVertexBuffer(*vData, bufferLayout);
		m_VAO->AddVertexBuffer(*m_VBO, PerInstanceData::VertexLayout());
		m_VAO->AddIndexBuffer(*iData);	// Bind the index buffer, no need to store it

		m_IndexCount = iData->GetCount();

		m_InstanceData = new PerInstanceData[InstanceCount];
	}

	void InstanceBatch::BeginBatch()
	{
		m_InstanceDataPtr = m_InstanceData;
		m_CurrentInstanceCount = 0;
	}

	void InstanceBatch::EndBatch()
	{
		if (m_InstanceDataPtr == nullptr)
			return;

		std::size_t bufferSize = m_CurrentInstanceCount * sizeof(PerInstanceData);
		m_VBO->SetData(m_InstanceData, 0, bufferSize);

		m_InstanceDataPtr = nullptr;
	}

	void InstanceBatch::Flush()
	{
		// Render everything
		// No need to do any thing in case there is no data
		if (m_InstanceDataPtr == m_InstanceData || m_CurrentInstanceCount == 0)
			return;

		Ref<Shader> shader = Renderer::GetShaderLibrary().GetShader("InstancedBatch");
		shader->Bind();

		// Bind all the textures
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			glBindTextureUnit(i, m_TextureIDs[i]);
		}

		m_VAO->Bind();

		glDrawElementsInstanced(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr, m_CurrentInstanceCount);

		m_CurrentInstanceCount = 0;
		m_TextureSlotIndex = 1;
	}

	void InstanceBatch::AddMesh(const Ref<Mesh3D>& mesh)
	{
		// This is only for one material right now. But will need to be updated for all the materials in the mesh
		Ref<Material> material = mesh->GetMaterial();
		const std::vector<Ref<const Texture2D>>& textures = material->GetTextures();
		
		if (IsFull() || (textures.size() > 0 && m_TextureSlotIndex == Renderer::MaxTextureImageUnits))
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		std::vector<float> texIds(textures.size(), 0.0f);

		for (int i = 0; i < textures.size(); i++)
		{
			uint32_t TextureId = textures[i]->GetID();
			float textureIndex = 0.0f;
			for (uint32_t textureSlot = 1; textureSlot < m_TextureSlotIndex; textureSlot++)
			{
				if (TextureId == m_TextureIDs[textureSlot])
				{
					textureIndex = (float)textureSlot;
					break;
				}
			}

			// If texture is not in the texture slots
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)m_TextureSlotIndex;
				m_TextureIDs[m_TextureSlotIndex++] = TextureId;
			}

			texIds[i] = textureIndex;
		}

		AddMesh_Internal(mesh, texIds);
	}

	// Need to check how to properly fill the data into the buffer
	// The instance data needs to be properly added with the per vertex data
	// Right now, the per vertex and per instance data are added separately using different strides 
	// which is causing the issues when data is read in the shader
	void InstanceBatch::AddMesh_Internal(const Ref<Mesh3D>& mesh, const std::vector<float>& texIds)
	{
		// Add Data into the mesh
		Ref<Material> mat = mesh->GetMaterial();

		// Matrix needs to be transposed because GPU expects matrices in data in column major format
		m_InstanceDataPtr->ModelMatrix = std::move(mesh->GetModelMatrix().Transpose());

		// Normal matrix is the transpose of inverse of model matrix
		// So no need to transpose
		m_InstanceDataPtr->NormalMatrix = std::move(Matrix3(std::move(mesh->GetModelMatrix().Inverse())));

		m_InstanceDataPtr->TintColor = mat->GetBaseColor();
		m_InstanceDataPtr->Reflectivity = mat->GetSpecularStrength();
		m_InstanceDataPtr->Shininess = mat->GetShininess();

		// For now only using one texture for each material, Material system needs a redesign
		m_InstanceDataPtr->TexIndex = texIds[0];

		m_InstanceDataPtr++;
		m_CurrentInstanceCount++;
	}
}