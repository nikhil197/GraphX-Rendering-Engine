#include "pch.h"
#include "Renderer3D.h"
#include "GL/glew.h"

#include "Renderer.h"
#include "Model/Mesh/Mesh3D.h"
#include "Shaders/Shader.h"
#include "Materials/Material.h"

#include "Batches/InstanceBatch.h"

#include "Utilities/HashUtil.h"

#include "VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"

#include "Entities/Terrain.h"

namespace GraphX
{
	using namespace GM;

	Renderer3D::Renderer3DData* Renderer3D::s_Data = nullptr;

	std::unordered_map<std::size_t, Ref<InstanceBatch>> Renderer3D::s_InstanceBatches;

	void Renderer3D::Init()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data == nullptr, "Renderer3D already Initialised");
		s_Data = new Renderer3D::Renderer3DData();

		s_Data->DebugData.VAO = CreateScope<VertexArray>();
		s_Data->DebugData.VBO = CreateScope<VertexBuffer>(8 * sizeof(Vector3));

		std::vector<unsigned int> Indices = {
				0, 1,
				1, 2,
				2, 3,
				3, 0,
				4, 5,
				5, 6,
				6, 7,
				7, 4,
				0, 4,
				1, 5,
				2, 6,
				3, 7
		};
		IndexBuffer ibo(Indices.data(), Indices.size());

		VertexBufferLayout layout = {
				{ BufferDataType::Float3 }
		};

		s_Data->DebugData.VAO->AddVertexBuffer(*(s_Data->DebugData.VBO), layout);
		s_Data->DebugData.VAO->AddIndexBuffer(ibo);

		int samplers[32]{};
		for (int i = 0; i < 32; i++)
		{
			samplers[i] = i;
		}

		Ref<Shader> instanceBatchShader = Renderer::GetShaderLibrary().GetShader("InstancedBatch");
		instanceBatchShader->Bind();
		instanceBatchShader->SetUniform1iv("u_Textures", 32, samplers);
	}

	void Renderer3D::Shutdown()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data != nullptr, "Renderer3D not Initialised!!");
		delete s_Data;
		s_Data = nullptr;
	}

	void Renderer3D::BeginScene()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data != nullptr, "Renderer3D not Initialised!!");
	}

	void Renderer3D::EndScene()
	{
		GX_PROFILE_FUNCTION()
	}

	void Renderer3D::Submit(const Ref<Mesh3D>& mesh)
	{
		s_Data->RenderQueue.emplace_back(mesh);
	}

	void Renderer3D::Submit(const Ref<Terrain>& terrain)
	{
		s_Data->RenderQueue.emplace_back(terrain->GetMesh());
	}

	void Renderer3D::Render()
	{
		s_Data->Stats.GeometryCount += s_Data->RenderQueue.size();

		while (!s_Data->RenderQueue.empty())
		{
			const Ref<Mesh3D>& mesh = s_Data->RenderQueue.front();
			s_Data->RenderQueue.pop_front();

			// Enable the object for rendering
			mesh->Enable();

			// Render the object
			const Ref<Material>& Mat = mesh->GetMaterial();
			Mat->Bind();
			const Ref<Shader>& shader = Mat->GetShader();	// NOTE: No Need to bind the shader again (Material binds the shader)
			
			// Set the transformation matrix
			const Matrix4& Model = mesh->GetModelMatrix();
			shader->SetUniformMat4f("u_Model", Model);

			// TODO: Improve this. There must be a better way to do this
			// Normal Transform Matrix (Could be done in the vertex shader, but more efficient here since vertex shader runs for each vertex)
			Matrix3 Normal = Matrix3(Model).Inverse().Transpose();
			shader->SetUniformMat3f("u_Normal", Normal);

			// Draw the object
			DrawCall(GL_TRIANGLES, mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT);
			//glDrawElements(GL_TRIANGLES, mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr);

			// Disable the mesh after drawing
			mesh->Disable();
			
			// Draw debug collision box
			if (GX_ENABLE_DEBUG_COLLISIONS_RENDERING)
			{
				RenderDebugCollisions(mesh->GetBoundingBox());
			}
		}
	}

	void Renderer3D::RenderInstanced()
	{
		// TODO: Needs to render debug collisions
		uint32_t queueSize = s_Data->RenderQueue.size();

		// Begin each instance batch;
		for (auto& itr : s_InstanceBatches)
		{
			itr.second->BeginBatch();
		}

		while (!s_Data->RenderQueue.empty())
		{
			const Ref<Mesh3D>& mesh = s_Data->RenderQueue.front();
			s_Data->RenderQueue.pop_front();

			// TODO: This is only accounting for the first material only
			std::size_t hash = mesh->GetMaterial()->GetHash(); 
			GM::Hash_Combine(hash, mesh->GetInstanceHash());

			auto itr = s_InstanceBatches.find(hash);
			if (itr != s_InstanceBatches.end())
			{
				itr->second->AddMesh(mesh);
			}
			else
			{
				Ref<InstanceBatch> batch = CreateRef<InstanceBatch>(hash, queueSize, mesh->GetVBO(), Vertex3D::VertexLayout(), mesh->GetIBO());

				batch->BeginBatch();
				batch->AddMesh(mesh);
				
				s_InstanceBatches[hash] = batch;
			}
		}

		// End and flush each instance batch;
		for (auto& itr : s_InstanceBatches)
		{
			itr.second->EndBatch();
			itr.second->Flush();
		}
	}

	void Renderer3D::Render(Shader& DepthShader)
	{
		DepthShader.Bind();

		s_Data->Stats.GeometryCount += s_Data->RenderQueue.size();

		for (unsigned int i = 0; i < s_Data->RenderQueue.size(); i++)
		{
			const Ref<Mesh3D>& Mesh = s_Data->RenderQueue.at(i);

			Mesh->Enable();

			// Set the transformation matrix
			const Matrix4& Model = Mesh->GetModelMatrix();
			DepthShader.SetUniformMat4f("u_Model", Model);

			// Draw the object
			DrawCall(GL_TRIANGLES, Mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT);
			//glDrawElements(GL_TRIANGLES, Mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr);

			Mesh->Disable();
		}
	}

	void Renderer3D::RenderDebugCollisions(const Ref<GM::BoundingBox>& Box)
	{
		GX_PROFILE_FUNCTION()
		// Todo: Figure out a better structure for debug drawing

		if (Renderer::s_DebugShader)
		{
			static std::vector<Vector3> VertexPositions(8);

			// Update the vertices for current box
			VertexPositions[0].x = Box->Min.x, VertexPositions[0].y = Box->Min.y, VertexPositions[0].z = Box->Max.z;
			VertexPositions[1].x = Box->Max.x, VertexPositions[1].y = Box->Min.y, VertexPositions[1].z = Box->Max.z;
			VertexPositions[2].x = Box->Max.x, VertexPositions[2].y = Box->Max.y, VertexPositions[2].z = Box->Max.z;
			VertexPositions[3].x = Box->Min.x, VertexPositions[3].y = Box->Max.y, VertexPositions[3].z = Box->Max.z;

			VertexPositions[4].x = Box->Min.x, VertexPositions[4].y = Box->Min.y, VertexPositions[4].z = Box->Min.z;
			VertexPositions[5].x = Box->Max.x, VertexPositions[5].y = Box->Min.y, VertexPositions[5].z = Box->Min.z;
			VertexPositions[6].x = Box->Max.x, VertexPositions[6].y = Box->Max.y, VertexPositions[6].z = Box->Min.z;
			VertexPositions[7].x = Box->Min.x, VertexPositions[7].y = Box->Max.y, VertexPositions[7].z = Box->Min.z;

			s_Data->DebugData.VBO->SetData(VertexPositions.data(), 8 * sizeof(Vector3));

			s_Data->DebugData.VAO->Bind();
			Renderer::s_DebugShader->Bind();
			s_Data->Stats.CollisionBoxes++;
			DrawCall(GL_LINES, 24, GL_UNSIGNED_INT);
		}
	}

	void Renderer3D::ResetStats()
	{
		// Using memset to reset the stats.. It is faster than setting each value manually since in a struct / class all variables will be continuous in memory
		memset(&s_Data->Stats.DrawCalls, 0, sizeof(Renderer3D::Statistics));
	}

	void Renderer3D::DrawCall(uint32_t RenderMode, uint32_t Count, uint32_t Type, const void* indices)
	{
		glDrawElements(RenderMode, Count, Type, indices);
		s_Data->Stats.DrawCalls++;
		s_Data->Stats.IndexCount += Count;
	}
}