#include "pch.h"
#include "Renderer.h"

#include "Renderer3D.h"
#include "Renderer2D.h"
#include "SimpleRenderer.h"
#include "RendererCommandList.h"

#include "Model\Mesh\Mesh2D.h"
#include "Model\Mesh\Mesh3D.h"
#include "Model/Cube.h"

#include "Core/Buffers/VertexBuffer.h"
#include "Core/Buffers/IndexBuffer.h"
#include "Core/VertexArray.h"

#include "Shaders\Shader.h"

#include "Textures/Texture2D.h"

#include "Entities/Camera.h"
#include "Entities/Skybox.h"

#include "Engine/Controllers/CameraController.h"

#include "Subsystems/Multithreading/Base/IRunnable.h"

#include "GL/glew.h"

namespace GraphX
{
#define CheckRenderer() {																		\
		if(!s_Renderer || !s_SceneInfo)															\
		{																						\
			GX_ENGINE_ASSERT(false, "Renderer Not Initialised before initialising a scene");	\
		}																						\
	}

	class RenderThread
		: public IRunnable
	{
		friend class Renderer;
	private:
		RendererCommandList* m_CommandList;

		// Whether it's time for the thread to exit
		bool m_Exit;

	public:
		RenderThread()
			: m_CommandList(nullptr), m_Exit(false)
		{}

		void ShutdownRendering()
		{
			m_Exit = true;
		}

		virtual bool Init() override
		{
			// TODO: Set the render context from here

			m_CommandList = new RendererCommandList();
			return true;
		}

		virtual uint32_t Run() override
		{
			while (!m_Exit)
			{
				// keep executing the render commands here
				m_CommandList->SwapCommandBuffers();

				while (!m_CommandList->m_CommandsForCurrentPass.empty())
				{
					RenderCommandBase* command = m_CommandList->m_CommandsForCurrentPass.front();
					command->ExecuteAndDestruct();
					m_CommandList->m_CommandsForCurrentPass.pop_front();
				}
			}

			return 0;
		}

		virtual void Stop() override
		{

		}

		virtual void Exit() override
		{
			// Shutdown the stuff here
		}
	};

	SimpleRenderer* Renderer::s_Renderer = nullptr;

	ShaderLibrary Renderer::s_ShaderLibrary;
	TextureLibrary Renderer::s_TextureLibrary;

	Renderer::SceneInfo* Renderer::s_SceneInfo = nullptr;
	SkyboxRenderData* Renderer::s_SkyboxData = nullptr;
	Ref<Shader> Renderer::s_DebugShader = nullptr;
	RunnableThread* Renderer::s_RenderThread = nullptr;
	RenderThread* Renderer::s_RenderThreadRunnable = new RenderThread();

	void Renderer::Init()
	{
		GX_PROFILE_FUNCTION()

		s_RenderThread = (RunnableThread*)IThread::Create(s_RenderThreadRunnable, EngineConstants::RenderThreadName);

		GX_ENGINE_ASSERT(s_RenderThread != nullptr, "Failed to create the Render Thread")

		s_Renderer = new SimpleRenderer();

		s_SceneInfo = new Renderer::SceneInfo();

		// Setup the skybox render data
		s_SkyboxData = new SkyboxRenderData();
		s_SkyboxData->VAO = CreateScope<VertexArray>();
		
		std::vector<uint32_t> indices = Cube::GetIndices();
		// Top face
		indices[6] = 7;
		indices[7] = 3;
		indices[8] = 6;
		indices[9] = 6;
		indices[10] = 3;
		indices[11] = 2;
		// Bottom face
		indices[18] = 0;
		indices[19] = 4;
		indices[20] = 1;
		indices[21] = 1;
		indices[22] = 4;
		indices[23] = 5;

		std::vector<GM::Vector3> vertices = Cube::GetVertexPositions();
		s_SkyboxData->VBO = CreateScope<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GM::Vector3));
		s_SkyboxData->IBO = CreateRef<IndexBuffer>(&indices[0], indices.size());

		s_SkyboxData->SkyboxShader = s_ShaderLibrary.Load("res/Shaders/SkyboxShader.glsl", "Skybox");

		VertexBufferLayout layout = {
			{ BufferDataType::Float3 }
		};

		s_SkyboxData->VAO->AddVertexBuffer(s_SkyboxData->VBO.operator*(), layout);
		s_SkyboxData->VAO->AddIndexBuffer(s_SkyboxData->IBO.operator*());

		// TODO: Bind this to the GX_ENABLE_DEBUG_COLLISIONS_RENDERING
		s_DebugShader = s_ShaderLibrary.Load("res/Shaders/DebugCollisionsShader.glsl", "Debug");
		
		// Create the white texture and add it to the texture library (MAY BE CONSIDER DOING IT IN THE CONSTRUCTOR)
		auto WhiteTexture = CreateRef<Texture2D>("White", 1, 1);
		uint32_t data = 0xffffffff;
		WhiteTexture->SetData(&data, sizeof(data));
		s_TextureLibrary.Add(WhiteTexture->GetName(), WhiteTexture);

		Renderer2D::Init();
		Renderer3D::Init();
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

		if (s_SkyboxData)
		{
			delete s_SkyboxData;
			s_SkyboxData = nullptr;
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

	void Renderer::SubmitRenderCommand(RenderCommandBase* Cmd)
	{
		s_RenderThreadRunnable->m_CommandList->Push(Cmd);
	}

	void Renderer::RenderSkybox(const Ref<class Skybox>& skybox)
	{
		GX_PROFILE_FUNCTION()

		s_SkyboxData->VAO->Bind();
		skybox->Enable();

		// TODO: Think about doing it using render commands
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);

		// Set the uniforms
		s_SkyboxData->SkyboxShader->Bind();
		s_SkyboxData->SkyboxShader->SetUniformMat4f("u_View", s_SceneInfo->SceneCamera->GetRotationViewMatrix());
		s_SkyboxData->SkyboxShader->SetUniformMat4f("u_Model", skybox->GetModel());
		s_SkyboxData->SkyboxShader->SetUniform4f("u_BlendColor", skybox->GetTintColor());

		const CameraController* CamControl = s_SceneInfo->SceneCamera->GetCameraController();
		if (CamControl->GetProjectionMode() == ProjectionMode::Perspective)
			s_SkyboxData->SkyboxShader->SetUniformMat4f("u_Projection", s_SceneInfo->SceneCamera->GetProjectionMatrix());

		s_SkyboxData->SkyboxShader->SetUniform1i("u_Skybox", EngineConstants::SkyboxBindingSlot);
		s_SkyboxData->SkyboxShader->SetUniform1f("u_BlendFactor", skybox->BlendFactor);

		RenderIndexed(s_SkyboxData->IBO.operator*());

		skybox->Disable();

		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
	}

	void Renderer::Render()
	{
		Renderer3D::Render();
		Renderer2D::Render();
	}

	void Renderer::RenderInstanced()
	{
		Renderer3D::RenderInstanced();

		// Renderer2D is not instanced for now
		//Render
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