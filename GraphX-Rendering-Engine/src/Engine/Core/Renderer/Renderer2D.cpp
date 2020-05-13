#include "pch.h"
#include "Renderer2D.h"
#include "Gl/glew.h"

#include "Engine/Core/Renderer/Renderer.h"

#include "Engine/Core/Shaders/Shader.h"
#include "Engine/Core/Materials/Material.h"

#include "Engine/Model/Mesh/Mesh2D.h"

#include "Engine/Core/Vertex.h"
#include "Engine/Core/VertexArray.h"
#include "Engine/Core/Buffers/VertexBuffer.h"
#include "Engine/Core/Buffers/IndexBuffer.h"
#include "Engine/Core/Textures/Texture2D.h"

#include "Engine/Entities/Camera.h"
#include "Engine/Entities/Particles/ParticleSystem.h"

#include "Engine/Core/Batches/Batch2D.h"
#include "Engine/Core/Batches/ParticleBatch.h"

namespace GraphX
{
	// Max quads in a batch
	static const uint32_t MaxQuadCount = 1000;

	// Max Particles in a batch
	static const uint32_t MaxParticlesCount = 5000;

	Renderer2D::Renderer2DData* Renderer2D::s_Data = nullptr;

	void Renderer2D::Init()
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(s_Data == nullptr, "Renderer2D already Initialised");
		s_Data = new Renderer2D::Renderer2DData();

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

		s_Data->Batch = CreateScope<Batch2D>(MaxQuadCount);
		s_Data->Batch->m_TextureIDs[0] = s_Data->WhiteTexture->GetID();
		
		s_Data->ParticleBatch = CreateScope<ParticleBatch>(MaxParticlesCount);
		s_Data->ParticleBatch->m_TextureIDs[0] = s_Data->WhiteTexture->GetID();

		s_Data->BatchShader = Renderer::GetShaderLibrary().Load("res/Shaders/BatchShader2D.glsl", "Batch2D");
		s_Data->ParticleBatchShader = Renderer::GetShaderLibrary().Load("res/Shaders/ParticleBatchShader.glsl", "ParticleBatch");

		// Setup texture slots in the shader
		int samplers[32];
		for (int i = 0; i < 32; i++)
		{
			samplers[i] = i;
		}

		s_Data->BatchShader->Bind();
		s_Data->BatchShader->SetUniform1iv("u_Textures", 32, samplers);

		s_Data->ParticleBatchShader->Bind();
		s_Data->ParticleBatchShader->SetUniform1iv("u_Textures", 32, samplers);
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

		if (GX_ENABLE_BATCH_RENDERING)
		{
			s_Data->Batch->BeginBatch();
		}

		Ref<Camera> Cam = Renderer::s_SceneInfo->SceneCamera;
		if (Cam->IsRenderStateDirty())
		{
			s_Data->TextureShader->Bind();
			s_Data->TextureShader->SetUniformMat4f("u_ProjectionView", Cam->GetProjectionViewMatrix());
			s_Data->ShadowDebugShader->SetUniformMat4f("u_ProjectionView", Cam->GetProjectionViewMatrix());
			
			if (GX_ENABLE_BATCH_RENDERING)
			{
				s_Data->BatchShader->Bind();
				s_Data->BatchShader->SetUniformMat4f("u_ProjectionView", Cam->GetProjectionViewMatrix());

				s_Data->ParticleBatchShader->Bind();
				s_Data->ParticleBatchShader->SetUniformMat4f("u_Projection", Cam->GetProjectionMatrix());
			}
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
		if (GX_ENABLE_BATCH_RENDERING)
		{
			s_Data->Batch->AddQuad(position, size, color);
		}
		else
		{
			s_Data->WhiteTexture->Bind();

			s_Data->TextureShader->Bind();
			s_Data->TextureShader->SetUniform4f("u_Tint", color);
			s_Data->TextureShader->SetUniform1i("u_Texture", 0);

			GM::Matrix4 model = GM::ScaleRotationTranslationMatrix(GM::Vector3(size, 1.0f), GM::Vector3::ZeroVector, position);
			s_Data->TextureShader->SetUniformMat4f("u_Model", model);

			s_Data->QuadVA->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			s_Data->WhiteTexture->UnBind();
		}
	}
		 
	void Renderer2D::DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const Ref<Texture2D>& texture, const GM::Vector4& tintColor, float tiling, uint32_t slot)
	{
		DrawQuad({ position, 0.0f }, size, texture, tintColor, tiling, slot);
	}

	void Renderer2D::DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture, const GM::Vector4& tintColor, float tiling, uint32_t slot)
	{
		if (GX_ENABLE_BATCH_RENDERING)
		{
			s_Data->Batch->AddQuad(position, size, texture, tintColor, tiling);
		}
		else
		{
			texture->Bind(slot);
			
			s_Data->TextureShader->Bind();
			s_Data->TextureShader->SetUniform4f("u_Tint", tintColor);
			s_Data->TextureShader->SetUniform1f("u_Tiling", tiling);
			s_Data->TextureShader->SetUniform1i("u_Texture", slot);

			GM::Matrix4 model = GM::ScaleRotationTranslationMatrix(GM::Vector3(size, 1.0f), GM::Vector3::ZeroVector, position);
			s_Data->TextureShader->SetUniformMat4f("u_Model", model);

			s_Data->QuadVA->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			texture->UnBind();
		}
	}

	void Renderer2D::DrawRotatedQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector3& rotation, const GM::Vector4& color)
	{
		DrawRotatedQuad({ position, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector3& rotation, const GM::Vector4& color)
	{
		if (GX_ENABLE_BATCH_RENDERING)
		{
			s_Data->Batch->AddQuad(position, size, rotation, color);
		}
		else
		{
			s_Data->WhiteTexture->Bind();
			
			s_Data->TextureShader->Bind();
			s_Data->TextureShader->SetUniform4f("u_Tint", color);
			s_Data->TextureShader->SetUniform1i("u_Texture", 0);

			GM::Matrix4 model = GM::ScaleRotationTranslationMatrix(GM::Vector3(size, 1.0f), rotation, position);
			s_Data->TextureShader->SetUniformMat4f("u_Model", model);

			s_Data->QuadVA->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			s_Data->WhiteTexture->UnBind();
		}
	}

	void Renderer2D::DrawRotatedQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector3& rotation, const Ref<Texture2D>& texture, const GM::Vector4& TintColor, float tiling, uint32_t slot)
	{
		DrawRotatedQuad({ position, 0.0f }, size, rotation, texture, TintColor, tiling, slot);
	}

	void Renderer2D::DrawRotatedQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector3& rotation, const Ref<Texture2D>& texture, const GM::Vector4& tintColor, float tiling, uint32_t slot)
	{
		if (GX_ENABLE_BATCH_RENDERING)
		{
			s_Data->Batch->AddQuad(position, size, rotation, texture, tintColor, tiling);
		}
		else
		{
			texture->Bind(slot);
			
			s_Data->TextureShader->Bind();
			s_Data->TextureShader->SetUniform4f("u_Tint", tintColor);
			s_Data->TextureShader->SetUniform1f("u_Tiling", tiling);
			s_Data->TextureShader->SetUniform1i("u_Texture", slot);

			GM::Matrix4 model = GM::ScaleRotationTranslationMatrix(GM::Vector3(size, 1.0f), rotation, position);
			s_Data->TextureShader->SetUniformMat4f("u_Model", model);

			s_Data->QuadVA->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			texture->UnBind();
		}
	}

	void Renderer2D::DrawDebugQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture, unsigned int slot)
	{
		s_Data->ShadowDebugShader->Bind();
		
		texture->Bind(slot);
		s_Data->ShadowDebugShader->SetUniform1i("u_Texture", slot);

		GM::Matrix4 model = GM::ScaleRotationTranslationMatrix(GM::Vector3(size, 1.0f), GM::Vector3::ZeroVector, position);
		s_Data->ShadowDebugShader->SetUniformMat4f("u_Model", model);

		s_Data->QuadVA->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		texture->UnBind();
	}

	void Renderer2D::RenderParticles(const std::unordered_map<std::string, Ref<ParticleSystem>>& ParticleSystems)
	{
		GX_PROFILE_FUNCTION()

		if(GX_ENABLE_BATCH_RENDERING)
		{
			const GM::Matrix4& ViewMatrix = Renderer::s_SceneInfo->SceneCamera->GetViewMatrix();
			const GM::Vector3 CamViewPos(ViewMatrix(0, 3), ViewMatrix(1, 3), ViewMatrix(2, 3));
			
			s_Data->ParticleBatch->BeginBatch();

			for (const auto& pair : ParticleSystems)
			{
				const Ref<ParticleSystem>& System = pair.second;
				const Ref<Texture2D>& Texture = System->GetConfig().ParticleProperties.Texture;

				if (Texture)
				{
					for (const Particle& particle : System.operator*())
					{
						if (particle.IsActive())
						{
							const ParticleProps& props = particle.GetProps();
							float scale = GM::Utility::Lerp(props.SizeBegin, props.SizeEnd, particle.GetLifeProgress());
							s_Data->ParticleBatch->AddParticle(props.Position + CamViewPos, { scale, scale }, Texture, GM::Vector4::UnitVector, particle.GetTexOffsets(), particle.GetBlendFactor());
						}
					}
				}
				else
				{
					for (const Particle& particle : System.operator*())
					{
						if (particle.IsActive())
						{
							const ParticleProps& props = particle.GetProps();
							float scale = GM::Utility::Lerp(props.SizeBegin, props.SizeEnd, particle.GetLifeProgress());
							GM::Vector4 color = GM::Utility::Lerp(props.ColorBegin, props.ColorEnd, particle.GetLifeProgress());
							s_Data->ParticleBatch->AddParticle(props.Position + CamViewPos, { scale, scale }, color);
						}
					}
				}
			}

			s_Data->ParticleBatch->EndBatch();
			s_Data->ParticleBatch->Flush();
		}
		else
		{
			const Ref<Shader>& ParticleShader = Renderer::GetShaderLibrary().GetShader("Particle");

			{
				// Pre Render Stuff
				GX_PROFILE_SCOPE("Particles - PreRender")
				
				ParticleShader->Bind();
				s_Data->QuadVA->Bind();

				glDepthMask(false);		// Don't render the particles to the depth buffer

				glEnable(GL_BLEND);		// To enable blending
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			{
				// Render Particles
				GX_PROFILE_SCOPE("Particles - Render")

				for (const auto& pair : ParticleSystems)
				{
					const Ref<ParticleSystem>& System = pair.second;
					const Ref<Texture2D>& Texture = System->GetConfig().ParticleProperties.Texture;
					if (Texture)
					{
						Texture->Bind();
						ParticleShader->SetUniform1i("u_ParticleTexture", 0);
						ParticleShader->SetUniform1i("u_TexAtlasRows", (int)Texture->GetRowsInTexAtlas());
					}
					else
					{
						// Bind the white texture in case the particle is using colors
						s_Data->WhiteTexture->Bind();
						ParticleShader->SetUniform1i("u_ParticleTexture", 0);
						ParticleShader->SetUniform1i("u_TexAtlasRows", 0);
					}

					for (const Particle& particle : System.operator*())
					{
						if (particle.IsActive())
						{
							particle.Enable(*ParticleShader);
							glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
						}
					}
				}
			}

			{
				// Post Render Stuff
				GX_PROFILE_SCOPE("Particles - PostRender")

				ParticleShader->UnBind();
				s_Data->QuadVA->UnBind();

				glDepthMask(true);
				glDisable(GL_BLEND);
			}
		}
	}

	void Renderer2D::Submit(const Ref<Mesh2D>& mesh)
	{
		s_Data->RenderQueue.emplace_back(mesh);
	}

	void Renderer2D::Render()
	{
		// Render the batch
		if (GX_ENABLE_BATCH_RENDERING)
		{
			s_Data->Batch->EndBatch();
			s_Data->Batch->Flush();
		}

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