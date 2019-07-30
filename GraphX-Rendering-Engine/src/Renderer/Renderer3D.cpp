#include "pch.h"

#include "Renderer3D.h"
#include "Model/Mesh/Mesh3D.h"
#include "Model/Model3D.h"
#include "Buffers/IndexBuffer.h"
#include "Shaders/Shader.h"

#include "Entities/Terrain.h"

namespace GraphX
{
	void Renderer3D::Submit(const Mesh3D* mesh)
	{
		m_RenderQueue.emplace_back(mesh);
	}

	void Renderer3D::Submit(const Model3D* model)
	{
		const std::vector<Mesh3D*>& meshes = model->GetMeshes();
		
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			const Mesh3D* mesh = meshes[i];
			m_RenderQueue.emplace_back(mesh);
		}
	}

	void Renderer3D::Submit(const Terrain* terrain)
	{
		m_RenderQueue.emplace_back(&terrain->GetMesh());
	}

	void Renderer3D::Render()
	{
		while (!m_RenderQueue.empty())
		{
			const Mesh3D* mesh = m_RenderQueue.front();
			m_RenderQueue.pop_front();

			// Enable the object for rendering
			mesh->Enable();

			// Render the object
			Shader& shader = mesh->GetShader();
			shader.Bind();
			
			// Set the transformation matrix
			GraphXMaths::Matrix4 Model = mesh->GetModelMatrix();
			shader.SetUniformMat4f("u_Model", Model);

			// Normal Transform Matrix (Could be done in the vertex shader, but more efficient here since vertex shader runs for each vertex)
			GraphXMaths::Matrix3 Normal = GraphXMaths::Matrix3(Model);
			shader.SetUniformMat3f("u_Normal", Normal);

			// Draw the object
			GLCall(glDrawElements(GL_TRIANGLES, mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr));

			// Disable the mesh after drawing
			mesh->Disable();
		}
	}

	void Renderer3D::Render(Shader& DepthShader)
	{
		for (unsigned int i = 0; i < m_RenderQueue.size(); i++)
		{
			const Mesh3D* Mesh = m_RenderQueue.at(i);

			Mesh->BindBuffers();

			// Set the transformation matrix
			const GraphXMaths::Matrix4& Model = Mesh->GetModelMatrix();
			DepthShader.SetUniformMat4f("u_Model", Model);

			// Draw the object
			GLCall(glDrawElements(GL_TRIANGLES, Mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr));

			Mesh->UnBindBuffers();
		}
	}
}