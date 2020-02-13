#include "pch.h"
#include "Renderer3D.h"
#include "GL/glew.h"

#include "Renderer.h"
#include "Model/Mesh/Mesh3D.h"
#include "Model/Model3D.h"
#include "Shaders/Shader.h"
#include "Materials/Material.h"

#include "VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"

#include "Entities/Terrain.h"

namespace GraphX
{
	using namespace GM;

	void Renderer3D::Submit(const Ref<Mesh3D>& mesh)
	{
		m_RenderQueue.emplace_back(mesh);
	}

	void Renderer3D::Submit(const Ref<Model3D>& model)
	{
		const Ref<std::vector<Ref<Mesh3D>>>& meshes = model->GetMeshes();
		
		for (unsigned int i = 0; i < meshes->size(); i++)
		{
			const Ref<Mesh3D>& mesh = meshes->at(i);
			m_RenderQueue.emplace_back(mesh);
		}
	}

	void Renderer3D::Submit(const Ref<Terrain>& terrain)
	{
		m_RenderQueue.emplace_back(terrain->GetMesh());
	}

	void Renderer3D::Render()
	{
		while (!m_RenderQueue.empty())
		{
			const Ref<Mesh3D>& mesh = m_RenderQueue.front();
			m_RenderQueue.pop_front();

			// Enable the object for rendering
			mesh->Enable();

			// Render the object
			const Ref<Material>& Mat = mesh->GetMaterial();
			Mat->Bind();
			const Ref<Shader>& shader = Mat->GetShader();	// NOTE: No Need to bind the shader again (Material binds the shader)
			
			// Set the transformation matrix
			const Matrix4& Model = mesh->GetModelMatrix();
			shader->SetUniformMat4f("u_Model", Model);

			// Normal Transform Matrix (Could be done in the vertex shader, but more efficient here since vertex shader runs for each vertex)
			Matrix3 Normal = Matrix3(Model);
			shader->SetUniformMat3f("u_Normal", Normal);

			// Draw the object
			glDrawElements(GL_TRIANGLES, mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr);

			// Disable the mesh after drawing
			mesh->Disable();
			
			// Draw debug collision box
			if (GX_ENABLE_DEBUG_COLLISIONS_RENDERING)
			{
				RenderDebugCollisions(mesh->GetBoundingBox(), Model);
			}
		}
	}

	void Renderer3D::Render(Shader& DepthShader)
	{
		DepthShader.Bind();

		for (unsigned int i = 0; i < m_RenderQueue.size(); i++)
		{
			const Ref<Mesh3D>& Mesh = m_RenderQueue.at(i);

			Mesh->BindBuffers();

			// Set the transformation matrix
			const Matrix4& Model = Mesh->GetModelMatrix();
			DepthShader.SetUniformMat4f("u_Model", Model);

			// Draw the object
			glDrawElements(GL_TRIANGLES, Mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr);

			Mesh->UnBindBuffers();
		}
	}

	void Renderer3D::RenderDebugCollisions(const Ref<GM::BoundingBox>& Box, const GM::Matrix4& Model)
	{
		// Todo: Figure out a better structure for debug drawing

		const Ref<Shader>& DebugShader = Renderer::GetDebugCollisionsShader();
		if (DebugShader)
		{
			static std::vector<Vector3> VertexPositions(8);
			
			static std::vector<unsigned int> Indices = {
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

			// Update the vertices for current box
			VertexPositions[0].x = Box->Min.x, VertexPositions[0].y = Box->Min.y, VertexPositions[0].z = Box->Max.z;
			VertexPositions[1].x = Box->Max.x, VertexPositions[1].y = Box->Min.y, VertexPositions[1].z = Box->Max.z;
			VertexPositions[2].x = Box->Max.x, VertexPositions[2].y = Box->Max.y, VertexPositions[2].z = Box->Max.z;
			VertexPositions[3].x = Box->Min.x, VertexPositions[3].y = Box->Max.y, VertexPositions[3].z = Box->Max.z;

			VertexPositions[4].x = Box->Min.x, VertexPositions[4].y = Box->Min.y, VertexPositions[4].z = Box->Min.z;
			VertexPositions[5].x = Box->Max.x, VertexPositions[5].y = Box->Min.y, VertexPositions[5].z = Box->Min.z;
			VertexPositions[6].x = Box->Max.x, VertexPositions[6].y = Box->Max.y, VertexPositions[6].z = Box->Min.z;
			VertexPositions[7].x = Box->Min.x, VertexPositions[7].y = Box->Max.y, VertexPositions[7].z = Box->Min.z;

			VertexArray DebugVAO;
			VertexBuffer DebugVBO(&VertexPositions[0], 8 * sizeof(Vector3));
			IndexBuffer DebugIBO(&Indices[0], Indices.size());

			VertexBufferLayout layout = {
				{ BufferDataType::Float3 }
			};

			DebugVAO.AddVertexBuffer(DebugVBO, layout);
			DebugVAO.AddIndexBuffer(DebugIBO);

			DebugVAO.Bind();
			DebugShader->Bind();
			DebugShader->SetUniformMat4f("u_Model", Model);
			glDrawElements(GL_LINES, DebugIBO.GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}
}