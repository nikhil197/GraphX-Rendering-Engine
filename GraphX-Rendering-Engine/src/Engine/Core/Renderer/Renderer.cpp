#include "pch.h"
#include "Renderer.h"

#include "Renderer3D.h"
#include "Renderer2D.h"
#include "SimpleRenderer.h"

#include "Model\Mesh\Mesh2D.h"
#include "Model\Mesh\Mesh3D.h"

#include "Shaders\Shader.h"

#include "Entities/Camera.h"
#include "Entities/Skybox.h"

#include "Engine/Controllers/CameraController.h"

#include "GL/glew.h"

namespace GraphX
{
#define CheckRenderer() {																		\
		if(!s_Renderer || !s_SceneInfo)										\
		{																						\
			GX_ENGINE_ASSERT(false, "Renderer Not Initialised before initialising a scene");	\
		}																						\
	}

	SimpleRenderer* Renderer::s_Renderer = nullptr;

	ShaderLibrary Renderer::s_ShaderLibrary;

	Renderer::SceneInfo* Renderer::s_SceneInfo = nullptr;
	Ref<Shader> Renderer::s_DebugShader = nullptr;

	void Renderer::Init()
	{
		GX_PROFILE_FUNCTION()

		s_Renderer   = new SimpleRenderer();

		Renderer2D::Init();
		Renderer3D::Init();

		s_SceneInfo = new Renderer::SceneInfo();

		// TODO: Bind this to the GX_ENABLE_DEBUG_COLLISIONS_RENDERING
		s_DebugShader = s_ShaderLibrary.Load("res/Shaders/DebugCollisionsShader.glsl", "Debug");
	}

	void Renderer::Shutdown()
	{
		GX_PROFILE_FUNCTION()

		Renderer2D::Shutdown();
		Renderer3D::Shutdown();

		if (!s_Renderer)
		{
			GX_ENGINE_WARN("Renderer::Shutdown called more than once.");
			return;
		}

		if (s_Renderer)
		{
			delete s_Renderer;
			s_Renderer = nullptr;
		}
		if (s_SceneInfo)
		{
			delete s_SceneInfo;
			s_SceneInfo = nullptr;
		}
		if (s_DebugShader)
		{
			s_DebugShader.reset();
		}
	}

	void Renderer::BeginScene(const Ref<Camera>& MainCamera)
	{
		CheckRenderer();

		s_SceneInfo->SceneCamera = MainCamera;

		// TODO: Update camera uniforms for all the shaders

		if (s_DebugShader)
		{
			s_DebugShader->Bind();
			s_DebugShader->SetUniformMat4f("u_ViewProjection", MainCamera->GetProjectionViewMatrix());
			s_DebugShader->SetUniform4f("u_DebugColor", 1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	void Renderer::EndScene()
	{
		s_SceneInfo->Reset();
	}

	void Renderer::Submit(const Ref<Mesh2D>& Mesh)
	{
		Renderer2D::Submit(Mesh);
	}

	void Renderer::Submit(const Ref<Mesh3D>& Mesh)
	{
		Renderer3D::Submit(Mesh);
	}

	void Renderer::Submit(const Ref<Terrain>& Terr)
	{
		Renderer3D::Submit(Terr);
	}

	void Renderer::RenderSkybox(const Ref<class Skybox>& skybox)
	{
		GX_PROFILE_FUNCTION()

		skybox->Enable();

		// TODO: Think about doing it using render commands
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);

		// Set the uniforms
		const Ref<Shader>& shader = s_ShaderLibrary.GetShader("Skybox");
		shader->Bind();
		shader->SetUniformMat4f("u_View", s_SceneInfo->SceneCamera->GetRotationViewMatrix());
		shader->SetUniformMat4f("u_Model", skybox->GetModel());
		shader->SetUniform4f("u_BlendColor", skybox->GetTintColor());

		const CameraController* CamControl = s_SceneInfo->SceneCamera->GetCameraController();

		if (CamControl->GetProjectionMode() == ProjectionMode::Perspective)
			shader->SetUniformMat4f("u_Projection", s_SceneInfo->SceneCamera->GetProjectionMatrix());

		shader->SetUniform1i("u_Skybox", skybox->GetBindingSlot());
		shader->SetUniform1f("u_BlendFactor", skybox->BlendFactor);

		RenderIndexed(*skybox->GetIBO());

		skybox->Disable();

		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
	}

	void Renderer::Render()
	{
		Renderer3D::Render();
		Renderer2D::Render();
	}

	void Renderer::RenderDepth(Shader& DepthShader)
	{
		Renderer3D::Render(DepthShader);
		Renderer2D::Render(DepthShader);
	}

	void Renderer::Render(unsigned int Count)
	{
		s_Renderer->Draw(Count);
	}

	void Renderer::RenderIndexed(const IndexBuffer& indexBuffer)
	{
		s_Renderer->DrawIndexed(indexBuffer);
	}
}