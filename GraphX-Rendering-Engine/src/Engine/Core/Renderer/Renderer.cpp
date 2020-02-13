#include "pch.h"
#include "Renderer.h"

#include "Renderer3D.h"
#include "Renderer2D.h"
#include "SimpleRenderer.h"

#include "Model\Mesh\Mesh2D.h"
#include "Model\Mesh\Mesh3D.h"

#include "Shaders\Shader.h"

#include "Entities\Camera.h"

namespace GraphX
{
#define CheckRenderer() {																\
		if(!s_Renderer || !s_Renderer2D || !s_Renderer3D || !s_SceneInfo)				\
		{																				\
			GX_ASSERT(false, "Renderer Not Initialised before initialising a scene");	\
		}																				\
	}

	Renderer3D* Renderer::s_Renderer3D = nullptr;
	Renderer2D* Renderer::s_Renderer2D = nullptr;
	SimpleRenderer* Renderer::s_Renderer = nullptr;

	ShaderLibrary Renderer::s_ShaderLibrary;

	Renderer::SceneInfo* Renderer::s_SceneInfo = nullptr;
	Ref<Shader> Renderer::s_DebugShader = nullptr;

	void Renderer::Initialize()
	{
		s_Renderer   = new SimpleRenderer();
		s_Renderer2D = new Renderer2D();
		s_Renderer3D = new Renderer3D();

		s_SceneInfo = new Renderer::SceneInfo();

		// TODO: Bind this to the GX_ENABLE_DEBUG_COLLISIONS_RENDERING
		s_DebugShader = s_ShaderLibrary.Load("res/Shaders/DebugCollisions.shader", "Debug");
	}

	void Renderer::CleanUp()
	{
		if (!s_Renderer && !s_Renderer2D && !s_Renderer3D)
		{
			GX_ENGINE_WARN("Renderer::CleanUp called more than once.");
			return;
		}

		if (s_Renderer)
		{
			delete s_Renderer;
			s_Renderer = nullptr;
		}
		if (s_Renderer2D)
		{
			delete s_Renderer2D;
			s_Renderer2D = nullptr;
		}
		if (s_Renderer3D)
		{
			delete s_Renderer3D;
			s_Renderer3D = nullptr;
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
		s_Renderer2D->Submit(Mesh);
	}

	void Renderer::Submit(const Ref<Mesh3D>& Mesh)
	{
		s_Renderer3D->Submit(Mesh);
	}

	void Renderer::Submit(const Ref<Model3D>& Model)
	{
		s_Renderer3D->Submit(Model);
	}

	void Renderer::Submit(const Ref<Terrain>& Terr)
	{
		s_Renderer3D->Submit(Terr);
	}

	void Renderer::Render()
	{
		s_Renderer3D->Render();
		s_Renderer2D->Render();
	}

	void Renderer::RenderDepth(Shader& DepthShader)
	{
		s_Renderer3D->Render(DepthShader);
		s_Renderer2D->Render(DepthShader);
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