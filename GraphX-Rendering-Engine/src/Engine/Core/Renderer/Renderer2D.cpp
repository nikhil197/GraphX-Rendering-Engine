#include "pch.h"
#include "Renderer2D.h"
#include "Gl/glew.h"

#include "Engine/Core/Renderer/Renderer.h"

#include "Engine/Core/Shaders/Shader.h"
#include "Engine/Core/Materials/Material.h"

#include "Engine/Model/Mesh/Mesh2D.h"
#include "Engine/Model/Mesh/Vertex.h"

#include "Engine/Core/VertexArray.h"
#include "Engine/Core/Buffers/VertexBuffer.h"
#include "Engine/Core/Buffers/IndexBuffer.h"
#include "Engine/Core/Textures/Texture2D.h"

#include "Engine/Entities/Camera.h"
#include "Engine/Entities/Particles/Particle.h"

namespace GraphX
{
	Renderer2D::Renderer2DStorage* Renderer2D::s_Data = nullptr;

	void Renderer2D::Init()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data == nullptr, "Renderer2D already Initialised");
		s_Data = new Renderer2D::Renderer2DStorage();

		s_Data->TextureShader = Renderer::GetShaderLibrary().Load("res/Shaders/TextureShader2D.glsl", "Texture2D");
		s_Data->ShadowDebugShader = Renderer::GetShaderLibrary().Load("res/Shaders/ShadowDebugShader.glsl", "ShadowDebug");

		s_Data->WhiteTexture = CreateScope<Texture2D>(1, 1);
		uint32_t data = 0xffffffff;
		s_Data->WhiteTexture->SetData(&data, sizeof(data));

		std::vector<Vertex2D> quadVertices = {
			{ GM::Vector3(-0.5f, -0.5f, 0.0f), GM::Vector2(0.0f, 0.0f) },
			{ GM::Vector3( 0.5f, -0.5f, 0.0f), GM::Vector2(1.0f, 0.0f) },
			{ GM::Vector3( 0.5f,  0.5f, 0.0f), GM::Vector2(1.0f, 1.0f) },
			{ GM::Vector3(-0.5f,  0.5f, 0.0f), GM::Vector2(0.0f, 1.0f) }
		};

		std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

		VertexBuffer vbo(&quadVertices[0], 4 * sizeof(Vertex2D));
		IndexBuffer ibo(&indices[0], 6);

		s_Data->QuadVA = CreateScope<VertexArray>();
		s_Data->QuadVA->AddVertexBuffer(vbo, Vertex2D::VertexLayout());
		s_Data->QuadVA->AddIndexBuffer(ibo);
	}

	void Renderer2D::Shutdown()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data != nullptr, "Renderer2D not Initialised!!");
		delete s_Data;
		s_Data = nullptr;
	}

	void Renderer2D::BeginScene()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data != nullptr, "Renderer2D not Initialised!!");

		if (Renderer::s_SceneInfo->SceneCamera->IsRenderStateDirty())
		{
			s_Data->TextureShader->Bind();
			s_Data->TextureShader->SetUniformMat4f("u_ProjectionView", Renderer::s_SceneInfo->SceneCamera->GetProjectionViewMatrix());
			s_Data->ShadowDebugShader->SetUniformMat4f("u_ProjectionView", Renderer::s_SceneInfo->SceneCamera->GetProjectionViewMatrix());
		}
	}
	
	void Renderer2D::EndScene()
	{
		GX_PROFILE_FUNCTION()
	}

	void Renderer2D::DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector4& color)
	{
		DrawQuad({ position, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector4& color)
	{
		s_Data->WhiteTexture->Bind();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetUniform4f("u_Tint", color);
		s_Data->TextureShader->SetUniform1i("u_Texture", 0);

		GM::Matrix4 model = GM::Translation(position) * GM::Scaling({ size, 1.0f });
		s_Data->TextureShader->SetUniformMat4f("u_Model", model);

		s_Data->QuadVA->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		s_Data->WhiteTexture->UnBind();
	}
		 
	void Renderer2D::DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const Ref<Texture2D>& texture, unsigned int slot)
	{
		DrawQuad({ position, 0.0f }, size, texture, slot);
	}

	void Renderer2D::DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture, unsigned int slot)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetUniform4f("u_Tint", GM::Vector4::UnitVector);
		
		texture->Bind(slot);
		s_Data->TextureShader->SetUniform1i("u_Texture", slot);
		
		GM::Matrix4 model = GM::Translation(position) * GM::Scaling({ size, 1.0f });
		s_Data->TextureShader->SetUniformMat4f("u_Model", model);

		s_Data->QuadVA->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		texture->UnBind();
	}

	void Renderer2D::DrawDebugQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture, unsigned int slot)
	{
		s_Data->ShadowDebugShader->Bind();
		
		texture->Bind(slot);
		s_Data->ShadowDebugShader->SetUniform1i("u_Texture", slot);

		GM::Matrix4 model = GM::Translation(position) * GM::Scaling({ size, 1.0f });
		s_Data->ShadowDebugShader->SetUniformMat4f("u_Model", model);

		s_Data->QuadVA->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		texture->UnBind();
	}

	void Renderer2D::RenderParticles(const std::vector<class Particle>& particles)
	{
		GX_PROFILE_FUNCTION()

		const Ref<Shader>& ParticleShader = Renderer::GetShaderLibrary().GetShader("Particle");
		
		{
			GX_PROFILE_SCOPE("Particles - PreRender")
			// Pre Render Stuff
			ParticleShader->Bind();
			s_Data->QuadVA->Bind();

			glDepthMask(false);		// Don't render the particles to the depth buffer

			glEnable(GL_BLEND);		// To enable blending
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		}

		{
			GX_PROFILE_SCOPE("Particles - Render")

			// Render Particles
			for (const Particle& particle : particles)
			{
				if (particle.IsActive())
				{
					particle.Enable(*ParticleShader);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
				}
			}
		}

		{
			GX_PROFILE_SCOPE("Particles - PostRender")
			// Post Render Stuff
			
			ParticleShader->UnBind();
			s_Data->QuadVA->UnBind();
				
			glDepthMask(true);
			glDisable(GL_BLEND);
		}
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