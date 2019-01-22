#include "pch.h"

#include "Renderer3D.h"
#include "Model/Mesh/Mesh3D.h"
#include "Model/Model3D.h"
#include "Buffers/IndexBuffer.h"
#include "Shaders/Shader.h"

namespace engine
{
	void Renderer3D::Submit(const Mesh3D* mesh)
	{
		m_RenderQueue.emplace_back(mesh);
	}

	void Renderer3D::Submit(const Model3D* model)
	{
		const std::vector<Mesh3D>& meshes = model->GetMeshes();
		
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			const Mesh3D* mesh = &meshes[i];
			m_RenderQueue.emplace_back(mesh);
		}
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

			// Set the transformation matrix
			shader.SetUniformMat4f("u_Model", mesh->GetTransformationMatrix());

			// Draw the object
			GLCall(glDrawElements(GL_TRIANGLES, mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr));

			// Disable the mesh after drawing
			mesh->Disable();
		}
	}
}