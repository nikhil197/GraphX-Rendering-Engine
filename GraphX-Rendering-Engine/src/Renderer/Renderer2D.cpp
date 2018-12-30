#include "pch.h"

#include "Renderer2D.h"
#include "Shaders/Shader.h"
#include "Model/Mesh/Mesh2D.h"
#include "Buffers/IndexBuffer.h"

namespace engine
{
	void Renderer2D::Submit(const Mesh2D& mesh)
	{
		m_RenderQueue.emplace_back(&mesh);
	}

	void Renderer2D::Render()
	{
		// While the queue is not empty
		while (!m_RenderQueue.empty())
		{
			const Mesh2D* mesh = m_RenderQueue.front();
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