#include "pch.h"
#include "Gl/glew.h"

#include "Renderer/Renderer.h"
#include "Renderer2D.h"
#include "Shaders/Shader.h"
#include "Materials/Material.h"
#include "Model/Mesh/Mesh2D.h"
#include "Buffers/IndexBuffer.h"
#include "Textures/Texture2D.h"

namespace GraphX
{
	Renderer2D::Renderer2DStorage* Renderer2D::s_Data = nullptr;

	void Renderer2D::Init()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data == nullptr, "Renderer2D already Initialised");
		s_Data = new Renderer2D::Renderer2DStorage();

		s_Data->TextureShader = Renderer::GetShaderLibrary().Load("res/Shaders/TextureShader2D.glsl", "Texture2D");

		s_Data->WhiteTexture = CreateRef<Texture2D>(1, 1);
	}

	void Renderer2D::Shutdown()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data != nullptr, "Renderer2D not Initialised!!");
		delete s_Data;
		s_Data = nullptr;
	}

	void Renderer2D::BeginScene(const Ref<class Camera>& MainCamera)
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data != nullptr, "Renderer2D not Initialised!!");

		s_Data->SceneCamera = MainCamera;
	}
	
	void Renderer2D::EndScene()
	{
		GX_PROFILE_FUNCTION()
	}

	void Renderer2D::DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector4& color) {

	}

	void Renderer2D::DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector4& color){

	}
		 
	void Renderer2D::DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const Ref<Texture2D>& texture){

	}

	void Renderer2D::DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture) {

	}

	void Renderer2D::Submit(const Ref<Mesh2D>& mesh)
	{
		s_Data->RenderQueue.emplace_back(mesh);
	}

	void Renderer2D::Render()
	{
		// While the queue is not empty
		while (!s_Data->RenderQueue.empty())
		{
			const Ref<Mesh2D>& mesh = s_Data->RenderQueue.front();
			s_Data->RenderQueue.pop_front();

			// Enable the object for rendering
			mesh->Enable();

			// Render the object
			const Ref<Material>& Mat = mesh->GetMaterial();
			Mat->Bind();
			const Ref<Shader>& shader = Mat->GetShader();	// NOTE: No Need to bind the shader again (Material binds the shader)

			// Set the transformation matrix
			const GM::Matrix4& Model = mesh->GetModelMatrix();
			shader->SetUniformMat4f("u_Model", Model);

			// Normal Transform Matrix (Could be done in the vertex shader, but more efficient here since vertex shader runs for each vertex)
			GM::Matrix3 Normal = GM::Matrix3(Model);
			shader->SetUniformMat3f("u_Normal", Normal);

			// Draw the object
			glDrawElements(GL_TRIANGLES, mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr);

			// Disable the mesh after drawing
			mesh->Disable();
		}
	}

	void Renderer2D::Render(Shader& DepthShader)
	{
		for (unsigned int i = 0; i < s_Data->RenderQueue.size(); i++)
		{
			const Ref<Mesh2D>& Mesh = s_Data->RenderQueue.at(i);

			Mesh->BindBuffers();

			// Set the transformation matrix
			GM::Matrix4 Model = Mesh->GetModelMatrix();
			DepthShader.SetUniformMat4f("u_Model", Model);

			// Draw the object
			glDrawElements(GL_TRIANGLES, Mesh->GetIBO()->GetCount(), GL_UNSIGNED_INT, nullptr);

			Mesh->UnBindBuffers();
		}
	}
}