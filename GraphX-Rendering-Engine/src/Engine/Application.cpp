#include "pch.h"
#include "Application.h"

#include "Engine/Core/Vertex.h"
#include "Engine/Core/VertexArray.h"
#include "Engine/Core/Shaders/Shader.h"
#include "Engine/Core/Materials/Material.h"

/* Buffers */
#include "Engine/Core/Buffers/VertexBuffer.h"
#include "Engine/Core/Buffers/VertexBufferLayout.h"
#include "Engine/Core/Buffers/IndexBuffer.h"
#include "Engine/Core/Buffers/FrameBuffer.h"

#include "Engine/Core/Textures/Texture2D.h"
#include "Engine/Core/Textures/SpriteSheet.h"

/* Renderer */
#include "Engine/Core/Renderer/Renderer.h"
#include "Engine/Core/Renderer/Renderer2D.h"
#include "Engine/Core/Renderer/Renderer3D.h"

/* Controllers */
#include "Engine/Controllers/CameraController.h"

/* Entities */
#include "Engine/Entities/Lights/PointLight.h"
#include "Engine/Entities/Lights/DirectionalLight.h"
#include "Engine/Entities/Camera.h"
#include "Engine/Entities/Skybox.h"
#include "Engine/Entities/Terrain.h"
#include "Engine/Entities/Particles/Particle.h"
#include "Engine/Entities/Particles/ParticleManager.h"
#include "Engine/Entities/Particles/ParticleSystem.h"

#include "Window.h"
#include "Timer/Clock.h"
#include "Gui/GraphXGui.h"

/* Events */
#include "Engine/Events/WindowEvent.h"
#include "Engine/Events/KeyboardEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/GUIEvent.h"

/* Input */
#include "Engine/Input/Keyboard.h"
#include "Engine/Input/Mouse.h"

/* Model */
#include "Engine/Model/ModelTypes.h"
#include "Engine/Model/Mesh/Mesh2D.h"
#include "Engine/Model/Mesh/Mesh3D.h"
#include "Engine/model/Cube.h"

/* Utils */
#include "Engine/Utilities/EngineUtil.h"
#include "Engine/Utilities/FileOpenDialog.h"
#include "Engine/Utilities/MousePicker.h"

#include "Subsystems/Multithreading/Multithreading.h"

#include "Engine/Utilities/AssetLoader/AssetLoader.h"

namespace GraphX
{
	using namespace GM;

	Application* Application::s_Instance = nullptr;

	Application::Application(const char* title, int width, int height)
		: m_Window(nullptr), m_Title(title), m_IsRunning(true), m_EngineDayTime(0.1f), m_SelectedObject2D(nullptr), m_SelectedObject3D(nullptr), m_SunLight(nullptr), m_ShadowBuffer(nullptr), m_DepthShader(nullptr), m_CameraController(nullptr), m_DaySkybox(nullptr), m_NightSkybox(nullptr), m_CurrentSkybox(nullptr), m_Shader(nullptr), m_DefaultMaterial(nullptr), m_Light(nullptr), m_DefaultTexture(nullptr)
	{
		GX_PROFILE_FUNCTION()

		GX_ENGINE_ASSERT(!s_Instance, "An Instance is already running!");
		s_Instance = this;

		/* Initialize subsystems*/

		// Initialize multi-threading
		Multithreading::Init();

		// Initialise the clock and the logging, and the input devices
		Log::Init();
		Clock::Init();
		Mouse::Init();
		Keyboard::Init();

		m_Window = new Window(WindowProps(m_Title, width, height));
		
		// Set the event callback with the window
		m_Window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		// Intialise the ImGui
		GraphXGui::Init(BIND_EVENT_FUNC(Application::OnEvent));

		// Initialise the renderer
		Renderer::Init();

		InitializeApplication();
	}

	void Application::InitializeApplication()
	{
		GX_PROFILE_FUNCTION()

		m_CameraController = CreateRef<CameraController>(GM::Vector3(-3.0f, 0.0f, 0.0f), GM::Vector3::ZeroVector, EngineConstants::UpAxis, (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), EngineConstants::NearPlane, EngineConstants::FarPlane);

		std::vector<std::string> SkyboxNames = { "right.png", "left.png" , "top.png" , "bottom.png" , "front.png" , "back.png" };
		m_DaySkybox  = CreateRef<Skybox>("res/Textures/Skybox/Day/", SkyboxNames, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_NightSkybox = CreateRef<Skybox>("res/Textures/Skybox/Night/", SkyboxNames, Vector4(0.5f, 0.5f, 0.5f, 1.0f), 0.0f, 0.f);

		m_CurrentSkybox = m_NightSkybox;
		
		m_SunLight = CreateRef<DirectionalLight>(GM::Vector4::UnitVector, GM::Vector3(-1.0f, -3.0f, 1.0f));
		m_Lights.emplace_back(m_SunLight);

		// Basic Lighting Shader 
		m_Shader = CreateRef<Shader>("res/Shaders/BasicLightingShader.glsl");
		m_Shader->Bind();
		m_Shaders.push_back(m_Shader);

		m_DefaultMaterial = CreateRef<Material>(m_Shader);

		m_Light = CreateRef<PointLight>(Vector3(0.0f, 50.0f, 50.0f), Vector4(1, 1, 1, 1));
		m_Lights.emplace_back(m_Light);

		m_ShadowBuffer = CreateRef<FrameBuffer>(m_Window->GetWidth(), m_Window->GetHeight(), FramebufferType::GX_FRAME_DEPTH);
		m_DepthShader = CreateRef<Shader>("res/Shaders/DepthShader.glsl");

		ParticleManager::Init(m_CameraController->GetCamera());

		m_DefaultTexture  = CreateRef<Texture2D>("res/Textures/stone.jpg");

		// Initialise the mouse picker
		MousePicker::Init(m_CameraController->GetCamera(), m_Window->GetWidth(), m_Window->GetHeight());
	}

	void Application::Run()
	{
		GX_PROFILE_FUNCTION()

		m_Shader->Bind();
		m_Shader->SetUniform1f("u_AmbientStrength", 0.1f);
		m_Shader->SetUniform1f("u_Shininess", 256.0f);
		m_Shader->SetUniform1f("u_Reflectivity", 1.0f);

		m_Shader->SetUniform3f("u_LightPos", m_Light->Position);
		m_Shader->SetUniform4f("u_LightColor", m_Light->Color);
		
		{
			GX_PROFILE_SCOPE("Load Scene")
			
			std::vector<Ref<const Texture2D>> textures(0);
			textures.push_back(m_DefaultTexture);

			Ref<Material> CubeMaterial = CreateRef<Material>(m_Shader);
			CubeMaterial->AddTexture(m_DefaultTexture);

			Ref<Cube> cube = CreateRef<Cube>(GM::Vector3(5.0f, -10.0f, 10.0f), GM::Rotator::ZeroRotator, GM::Vector3::UnitVector, CubeMaterial);
			m_Objects3D.emplace_back(cube);
			cube->bShowDetails = true;

			Ref<Terrain> ter = CreateRef<Terrain>(250, 250, 2.0f, std::vector<std::string>({ "res/Textures/Terrain/Grass.png", "res/Textures/Terrain/GrassFlowers.png", "res/Textures/Terrain/Mud.png", "res/Textures/Terrain/Path.png" }), "res/Textures/Terrain/BlendMap.png", Vector3(249.0f, -249.0f, 10.0f), Vector2(1.0f, 1.0f));
			m_Terrain.emplace_back(ter);

			// Load Trees
			Ref<Material> TreeMaterial = CreateRef<Material>(m_Shader);
			TreeMaterial->AddTexture(CreateRef<const Texture2D>("res/Textures/tree.png"));
			
			std::future<Ref<Mesh3D>> ft = Async<Ref<Mesh3D>>(AsyncExecutionPolicy::ThreadPool, std::bind(&Mesh3D::Load, "res/Models/tree.obj", TreeMaterial));
			Ref<Mesh3D> TreeMesh = ft.get();
			//Ref<Mesh3D> TreeMesh = Mesh3D::Load("res/Models/tree.obj", TreeMaterial);
			TreeMesh->Scale *= 2.5f;
			unsigned int NumTree = 100;
			for (unsigned int i = 0; i < NumTree; i++)
			{
				Vector3 Position((2 * EngineUtil::Rand<float>() - 1) * ter->GetWidth() / 2, (2 * EngineUtil::Rand<float>() - 1) * ter->GetDepth() / 2, 0.0f);
				TreeMesh->Position = Position;
				TreeMesh->Rotation.Roll = -90.0f;
				m_Objects3D.emplace_back(CreateRef<Mesh3D>(TreeMesh.operator*()));
			}

			// Load Low poly Trees
			Ref<Material> LowPolyTreeMaterial = CreateRef<Material>(m_Shader);
			LowPolyTreeMaterial->AddTexture(CreateRef<const Texture2D>("res/Textures/lowPolyTree.png"));

			Ref<Mesh3D> LowPolyTreeMesh = Mesh3D::Load("res/Models/lowPolyTree.obj", LowPolyTreeMaterial);
			LowPolyTreeMesh->Scale = Vector3::UnitVector;
			NumTree = 10;
			for (unsigned int i = 0; i < NumTree; i++)
			{
				Vector3 Position((2 * EngineUtil::Rand<float>() - 1) * ter->GetWidth() / 2, (2 * EngineUtil::Rand<float>() - 1) * ter->GetDepth() / 2, 0.0f);
				LowPolyTreeMesh->Position = Position;
				LowPolyTreeMesh->Rotation.Roll = 90.0f;
				m_Objects3D.emplace_back(CreateRef<Mesh3D>(LowPolyTreeMesh.operator*()));
			}

			// Load Stall
			Ref<Material> StallMaterial = CreateRef<Material>(m_Shader);
			StallMaterial->AddTexture(CreateRef<const Texture2D>("res/Textures/stallTexture.png"));

			Ref<Mesh3D> StallMesh = Mesh3D::Load("res/Models/stall.obj", StallMaterial);
			StallMesh->Position = Vector3(100.0f, -75.0f, 0.0f);
			StallMesh->Rotation.Roll = 90.0f;
			m_Objects3D.emplace_back(StallMesh);

			m_Shader->UnBind();
		}
		
		Ref<SpriteSheet> particleSpriteSheet = CreateRef<SpriteSheet>("res/Textures/Particles/particleAtlas.png", 16, GM::Vector2(32.0f, 32.0f));
		ParticleProps particleProperties;
		particleProperties.Texture = particleSpriteSheet;
		particleProperties.Velocity = GM::Vector3(2.0f);
		particleProperties.GravityEffect = 0.5f;
		particleProperties.LifeSpan = 2.0f;
		particleProperties.SizeBegin = particleProperties.SizeEnd = 1.0f;

		ParticleSystemConfig config;
		config.ParticleProperties = particleProperties;
		config.ParticlesPerSec = 50;
		config.VelocityVariation = GM::Vector3(0.5f);
		config.LifeSpanVariation = 0.4f;
		config.SizeVariation = 0.5f;
		config.GravityVariation = 1.0f;
		
		Ref<ParticleSystem> particleSys = CreateRef<ParticleSystem>("Fire Texture ParticleSystem", config, GM::Vector3::ZeroVector);
		ParticleManager::AddParticleSystem(particleSys);
		
		ParticleProps particleProperties2;
		particleProperties2.ColorBegin = GM::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		particleProperties2.ColorEnd = GM::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		particleProperties2.Velocity = GM::Vector3(2.0f);
		particleProperties2.GravityEffect = 0.5f;
		particleProperties2.LifeSpan = 2.0f;
		particleProperties2.SizeBegin = particleProperties.SizeEnd = 1.0f;

		config.ParticleProperties = particleProperties2;
		Ref<ParticleSystem> particleSys2 = CreateRef<ParticleSystem>("Color ParticleSystem", config, GM::Vector3(50.0f, 70.0f, 10.0f));
		ParticleManager::AddParticleSystem(particleSys2);

		{
			GX_PROFILE_SCOPE("Initialise Resources")

			// Initialise the mesh resources
			for (size_t i = 0; i < m_Objects3D.size(); i++)
				m_Objects3D[i]->InitResources();

			// Initialise the terrain resources
			for (size_t i = 0; i < m_Terrain.size(); i++)
				m_Terrain[i]->InitResources();
		}

		// For the purpose of fps count
		int times = 0;
		float then = Clock::GetClock()->GetEngineTime();

		// Draw while the window doesn't close
		while (m_IsRunning)
		{
			GX_PROFILER_NEW_FRAME()
			GX_PROFILE_SCOPE("Application::Frame")

			// Frame Time in seconds
			float DeltaTime = Clock::GetClock()->GetDeltaTime();
			
			// Tick the clock every frame to get the delta time
			Clock::GetClock()->Tick();

			// Calculate the fps
			times++;
			float now = Clock::GetClock()->GetEngineTime();
			if ((now - then) > 1.0f)
			{
				GX_ENGINE_INFO("Frame Rate: {0} FPS", times);
				then = now;
				times = 0;

				Renderer2D::Statistics Stats = Renderer2D::GetStats();
				GX_ENGINE_INFO("Renderer2D Stats: {0} Draw Calls, {1} Quad Count", Stats.DrawCalls, Stats.QuadCount);
			}

			// No need to update or render stuff if the application (window) is minimised
			if (!m_IsMinimised)
			{
				// Reset Stats at the beginning of the frame 
				Renderer2D::ResetStats();

				{
					GX_PROFILE_SCOPE("Frame-Update")

					if (GX_ENABLE_PARTICLE_EFFECTS)
					{
						ParticleManager::SpawnParticles(DeltaTime);
					}
					
					// Load New Resources Before Updating anything
					LoadNewResources();

					// Update all the elements of the scene
					Update(DeltaTime);

					// Update the Gui
					GraphXGui::Update();
					
					// Clear the window 
					m_Window->Clear();
				}

				{
					GX_PROFILE_SCOPE("Frame-Render")

					// Start a scene
					Renderer::BeginScene(m_CameraController->GetCamera());

					Renderer2D::BeginScene();
					Renderer3D::BeginScene();

					for (unsigned int i = 0; i < m_Objects3D.size(); i++)
						Renderer::Submit(m_Objects3D[i]);

					// Calculate the shadow maps
					if (GX_ENABLE_SHADOWS)
					{
						RenderShadowMap();
					}

					// Draw the debug quad to show the depth map
					//Renderer2D::DrawQuad({ -10.0f, 10.0f, 0.0f }, 5 * GM::Vector2::UnitVector, m_DefaultTexture);

					Renderer::RenderSkybox(m_CurrentSkybox);
					
					// Bind the shader and draw the objects
					m_Shader->Bind();
					m_ShadowBuffer->BindDepthMap(EngineConstants::ShadowMapTextureSlot);
					ConfigureShaderForRendering(*m_Shader);

					Render2DScene();
					RenderScene();

					if (GX_ENABLE_PARTICLE_EFFECTS)
					{
						ParticleManager::RenderParticles();
					}

					Renderer2D::EndScene();
					Renderer3D::EndScene();

					// End the scene
					Renderer::EndScene();

					// Renders ImGUI
					RenderGui();
				}
			}

			//Update the mouse
			Mouse::GetMouse()->Update();

			//Poll events and swap buffers
			m_Window->OnUpdate();
		}
	}

	void Application::LoadNewResources()
	{
		// Load new 3D Meshes
		{
			// Lock the queue so that no other thread can access it
			std::lock_guard<std::mutex> lock(m_Mesh3DMutex);
			while (!m_Loaded3DMeshes.empty())
			{
				Ref<Mesh3D> LoadedMesh3D = m_Loaded3DMeshes.front();
				m_Loaded3DMeshes.pop_front();

				// Initialise resources and add into the list of meshes
				LoadedMesh3D->InitResources();
				m_Objects3D.emplace_back(LoadedMesh3D);
			}
		}
	}

	void Application::Update(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		// Update the camera
		m_CameraController->Update(DeltaTime);
		
		// Update the lights
		{
			GX_PROFILE_SCOPE("Update::Lights")

			for (size_t i = 0; i < m_Lights.size(); i++)
				m_Lights[i]->Update(DeltaTime);
		}

		// Update the meshes
		{
			GX_PROFILE_SCOPE("Update::2D Meshes")

			for (size_t i = 0; i < m_Objects2D.size(); i++)
				m_Objects2D[i]->Update(DeltaTime);
		}
		
		// Update lights
		{
			GX_PROFILE_SCOPE("Update::3D Meshes")

			for (size_t i = 0; i < m_Objects3D.size(); i++)
				m_Objects3D[i]->Update(DeltaTime);
		}

		// Update Terrain
		{
			GX_PROFILE_SCOPE("Update::Terrain")

			for (size_t i = 0; i < m_Terrain.size(); i++)
				m_Terrain[i]->Update(DeltaTime);
		}

		ParticleManager::Update(DeltaTime);

		DayNightCycleCalculations(DeltaTime);

		m_CurrentSkybox->Update(DeltaTime);

		if (m_CameraController->GetCamera()->IsRenderStateDirty())
		{
			GX_PROFILE_SCOPE("Update Camera Uniforms")

			for (unsigned int i = 0; i < m_Shaders.size(); i++)
			{
				const Ref<Shader>& shader = m_Shaders.at(i);
				if (!shader)
				{
					m_Shaders.erase(m_Shaders.begin() + i);		// TODO : Fix Memory leak
					continue;
				}
				shader->Bind();
				shader->SetUniform3f("u_CameraPos", m_CameraController->GetCameraPosition());

				//TODO: Uniforms need to be set in the renderer
				shader->SetUniformMat4f("u_ProjectionView", m_CameraController->GetCamera()->GetProjectionViewMatrix());
			}

			// Update the terrain Material shader (TODO: Find a better way)
			for (unsigned int i = 0; i < m_Terrain.size(); i++)
			{
				const Ref<Shader>& shader = m_Terrain[i]->GetMaterial()->GetShader();
				shader->Bind();
				shader->SetUniform3f("u_CameraPos", m_CameraController->GetCameraPosition());
				shader->SetUniformMat4f("u_ProjectionView", m_CameraController->GetCamera()->GetProjectionViewMatrix());
			}
		}
	}

	void Application::RenderShadowMap()
	{
		GX_PROFILE_FUNCTION()

		// Render the shadow maps
		m_DepthShader->Bind();
		m_DepthShader->SetUniformMat4f("u_LightSpaceMatrix", m_SunLight->GetShadowInfo()->LightViewProjMat);
		m_ShadowBuffer->Bind();
	
		m_Window->ClearDepthBuffer();

		RenderScene(true);

		m_ShadowBuffer->UnBind();
	}

	void Application::RenderScene(bool IsShadowPhase)
	{
		GX_PROFILE_FUNCTION()

		if (IsShadowPhase)
		{
			Renderer::RenderDepth(*m_DepthShader);
		}
		else
		{
			Renderer::Render();
		}
		
		RenderTerrain(IsShadowPhase);
	}

	void Application::Render2DScene()
	{
		Renderer2D::DrawQuad({ -4.0f, 5.0f, -10.f }, { 5.0f, 5.0f }, GM::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		Renderer2D::DrawRotatedQuad({ -10.0f, 5.0f, -10.f }, { 5.0f, 5.0f }, {0.0f, 0.0f, 45.0f}, GM::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		Renderer2D::DrawQuad({ 4.0f, 5.0f, -10.f }, { 5.0f, 5.0f }, m_DefaultTexture);

		if (GX_ENABLE_SHADOWS)
		{
			// Draw the debug quad to show the depth map
			Renderer2D::DrawDebugQuad({-10.0f, 10.0f, 0.0f}, 5 * GM::Vector2::UnitVector, m_ShadowBuffer->GetDepthMap(), EngineConstants::ShadowMapTextureSlot);
		}
	}

	void Application::RenderTerrain(bool IsShadowPhase)
	{
		GX_PROFILE_FUNCTION()

		// TODO: Design a better API
		Ref<Shader> shader = m_DepthShader;

		for (unsigned int i = 0; i < m_Terrain.size(); i++)
		{
			Ref<Terrain> terrain = m_Terrain[i];

			// Configure the terrain shaders
			terrain->Enable();
			if (!IsShadowPhase)
			{
				shader = terrain->GetMaterial()->GetShader();
				ConfigureShaderForRendering(*shader);
			}

			shader->Bind();

			// Set the transformation matrix
			GM::Matrix4 Model = terrain->GetMesh()->GetModelMatrix();
			shader->SetUniformMat4f("u_Model", Model);

			// Render the Terrain
			Renderer::RenderIndexed(*terrain->GetMesh()->GetIBO());

			terrain->Disable();
		}
	}

	void Application::RenderGui()
	{
		GX_PROFILE_FUNCTION()

		GraphXGui::DetailsWindow(m_Objects3D[0]);
		if (m_SelectedObject3D != nullptr)
			GraphXGui::DetailsWindow(m_SelectedObject3D, "Selected Object");

		GraphXGui::LightProperties(m_Light);
		GraphXGui::CameraProperties(m_CameraController);
		GraphXGui::Models();
		if (m_Terrain.size() > 0 && m_Terrain[0] != nullptr)
		{
			GraphXGui::TerrainDetails(m_Terrain[0]);
		}
		GraphXGui::GlobalSettings(m_CurrentSkybox, m_EngineDayTime, m_SunLight->Intensity, GX_ENABLE_PARTICLE_EFFECTS);
		GraphXGui::Render();
	}

	void Application::ConfigureShaderForRendering(Shader& shader)
	{
		GX_PROFILE_FUNCTION()

		shader.SetUniform1i("u_ShadowMap", EngineConstants::ShadowMapTextureSlot);
		shader.SetUniform3f("u_LightPos", m_Light->Position);
		shader.SetUniform4f("u_LightColor", m_Light->Color);

		if(GX_ENABLE_SHADOWS)
			shader.SetUniformMat4f("u_LightSpaceMatrix", m_SunLight->GetShadowInfo()->LightViewProjMat);

		m_SunLight->Enable(shader, "u_LightSource");
	}

	void Application::OnEvent(Event& e)
	{
		GX_PROFILE_FUNCTION()

		// TODO: Send the event to all the layers (once the layer system is in place)
		m_CameraController->OnEvent(e);

		bool handled = false;
		EventDispatcher dispatcher(e);

		if (e.IsInCategory(EventCategory::GX_EVENT_CATEGORY_WINDOW))
		{
			// handle the window events
			if (!handled)
			{
				handled = dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUNC(Application::OnWindowResize));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<WindowMovedEvent>(BIND_EVENT_FUNC(Application::OnWindowMoved));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT_FUNC(Application::OnWindowFocus));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<WindowLostFocusEvent>(BIND_EVENT_FUNC(Application::OnWindowLostFocus));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowClose));
			}
		}
		else if (e.IsInCategory(EventCategory::GX_EVENT_CATEGORY_KEYBOARD))
		{
			if (!handled)
			{
				handled = dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(Application::OnKeyPressed));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FUNC(Application::OnKeyReleased));
			}
		}
		else if (e.IsInCategory(EventCategory::GX_EVENT_CATEGORY_MOUSE))
		{
			// TODO: Find a better way to do this
			if (GraphXGui::WantsMouseInput())
				return;
			if (!handled)
			{
				handled = dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(Application::OnMouseButtonPressed));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNC(Application::OnMouseButtonReleased));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNC(Application::OnMouseMoved));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(Application::OnMouseScrolled));
			}
		}
		else if (e.IsInCategory(EventCategory::GX_EVENT_CATEGORY_GUI))
		{
			if (!handled)
			{
				handled = dispatcher.Dispatch<AddTextureEvent>(BIND_EVENT_FUNC(Application::OnAddTexture));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<AddModelEvent>(BIND_EVENT_FUNC(Application::OnAddModel));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<CreateTerrainEvent>(BIND_EVENT_FUNC(Application::OnCreateTerrain));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<CameraFOVChangedEvent>(BIND_EVENT_FUNC(Application::OnCameraFOVChanged));
			}
			if (!handled)
			{
				handled = dispatcher.Dispatch<CameraProjectionModeChange>(BIND_EVENT_FUNC(Application::OnCameraProjectionModeChanged));
			}
		}
		
		// Raise an error if the event is not handled
		if (!handled)
		{
			GX_ENGINE_ERROR("Unhandled Event: \"{0}\" ", e);
		}
	}

	void Application::OnMesh3DLoad(const Ref<Mesh3D>& Mesh)
	{
		if (Mesh != nullptr)
		{
			std::lock_guard<std::mutex> lock(m_Mesh3DMutex);
			m_Loaded3DMeshes.emplace_back(Mesh);
			GX_ENGINE_TRACE("Mesh Loaded on thread {0}", std::this_thread::get_id());
		}
	}

	void Application::DayNightCycleCalculations(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		// Convert the time into hours
		float EngineTime = Clock::GetClock()->GetEngineTime() / (60.0f * 60.0f);
		int Days = (int)(EngineTime / m_EngineDayTime);
		float DayTime = EngineTime - m_EngineDayTime * Days;
		float TimeOfDay = DayTime * 24.0f / m_EngineDayTime;

		if (TimeOfDay >= DayTime::GX_START && TimeOfDay < DayTime::GX_EARLY_MORNING)
		{
			m_CurrentSkybox->BlendFactor = 0.0f;
		}
		else if (TimeOfDay >= DayTime::GX_EARLY_MORNING && TimeOfDay < DayTime::GX_SUNRISE)
		{
			TimeOfDay -= DayTime::GX_EARLY_MORNING;
			m_CurrentSkybox->BlendFactor = 0.5f;
		}
		else if (TimeOfDay >= DayTime::GX_SUNRISE && TimeOfDay < DayTime::GX_MORNING)
		{
			TimeOfDay = DayTime::GX_MORNING - TimeOfDay;
			m_CurrentSkybox = m_DaySkybox;
			m_SunLight->Color = GM::Vector4(0.5f, 0.5f, 0.0f, 1.0f);
			m_CurrentSkybox->BlendFactor = 0.6f;
		}
		else if (TimeOfDay >= DayTime::GX_MORNING && TimeOfDay < DayTime::GX_AFTERNOON)
		{
			TimeOfDay = DayTime::GX_AFTERNOON - TimeOfDay;
			m_CurrentSkybox->BlendFactor = 0.0f;
		}
		else if (TimeOfDay >= DayTime::GX_AFTERNOON && TimeOfDay < DayTime::GX_EVENING)
		{
			TimeOfDay -= DayTime::GX_AFTERNOON;
			m_CurrentSkybox->BlendFactor = 0.3f;
		}
		else if (TimeOfDay >= DayTime::GX_EVENING && TimeOfDay < DayTime::GX_NIGHT)
		{
			TimeOfDay -= DayTime::GX_EVENING;
			m_CurrentSkybox->BlendFactor = 0.7f;
		}
		else
		{
			TimeOfDay = DayTime::GX_NIGHT - TimeOfDay;
			m_CurrentSkybox = m_NightSkybox;
			m_SunLight->Color = GM::Vector4::UnitVector;
			m_CurrentSkybox->BlendFactor = 0.2f;
		}

		if (TimeOfDay > 0.0f)
		{
			m_CurrentSkybox->BlendFactor *= (TimeOfDay / 3.0f);
		}

		float angle = DeltaTime * 25.0f / (m_EngineDayTime * 10.0f);
		GM::RotationMatrix rotation(angle, EngineConstants::UpAxis);
		m_SunLight->Direction = GM::Vector3(rotation * GM::Vector4(m_SunLight->Direction, 1.0f));
	}

	void Application::PickObject()
	{
		const GM::Vector3& ForwardAxis = m_CameraController->GetForwardAxis();
		const GM::Vector3& CameraPos = m_CameraController->GetCamera()->GetPosition();
		const GM::Vector3& PickerRay = MousePicker::Get()->GetPickerRay();
		m_SelectedObject3D = nullptr;
		for (size_t i = 0; i < m_Objects3D.size(); i++)
		{
			const Ref<Mesh3D>& Mesh = m_Objects3D[i];
			// Ignore if the object is behind the camera
			if (GM::Vector3::DotProduct(Mesh->Position - CameraPos, PickerRay) <= 0)
				continue;

			// Check for the Picker Ray and Bounding box intersection
			if (BoundingBox::RayIntersectionTest(*(Mesh->GetBoundingBox()), CameraPos, PickerRay))
			{
				Mesh->bShowDetails = true;
				m_SelectedObject3D = Mesh;
				break;
			}
		}
	}

#pragma region eventHandlers

	bool Application::OnWindowResize(WindowResizedEvent& e)
	{
		// If either of the width or height is zero, it means the window is minimised
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_IsMinimised = true;
			return true;
		}

		m_IsMinimised = false;

		// Set the new size of the window
		m_Window->OnResize();
		
		return true;
	}

	bool Application::OnWindowMoved(WindowMovedEvent& e)
	{
		// Stuff to be added later
		return true;
	}

	bool Application::OnWindowFocus(WindowFocusEvent& e)
	{
		// Stuff to be added later
		return true;
	}

	bool Application::OnWindowLostFocus(WindowLostFocusEvent& e)
	{
		// Stuff to be added later
		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

	bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		Mouse::GetMouse()->OnEvent(e);

		// DO MOUSE PICKING HERE
		if (e.GetButton() == MouseButton::GX_MOUSE_LEFT && !Mouse::GetMouse()->IsRightButtonPressed())
		{
			PickObject();

			// TODO: Terrain and skybox picking logic
		}

		return true;
	}

	bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		Mouse::GetMouse()->OnEvent(e);
		return true;
	}

	bool Application::OnMouseMoved(MouseMovedEvent& e)
	{
		Mouse::GetMouse()->OnEvent(e);
		return true;
	}

	bool Application::OnMouseScrolled(MouseScrolledEvent& e)
	{
		Mouse::GetMouse()->OnEvent(e);
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		Keyboard::GetKeyboard()->OnEvent(e);
		return true;
	}

	bool Application::OnKeyReleased(KeyReleasedEvent& e)
	{
		Keyboard::GetKeyboard()->OnEvent(e);
		return true;
	}

	bool Application::OnAddTexture(AddTextureEvent& e)
	{
		FileOpenDialog dialog(ResourceType::TEXTURES);
		dialog.Show();

		std::string TexName = EngineUtil::ToByteString(dialog.GetAbsolutePath());
		Ref<Texture2D> texture = CreateRef<Texture2D>(TexName);

		if (m_SelectedObject3D)
		{
			Ref<Material> SelectedMat = m_SelectedObject3D->GetMaterial();
			SelectedMat->AddTexture(texture);
		}
		else if (m_SelectedObject2D)
		{
			Ref<Material> SelectedMat = m_SelectedObject2D->GetMaterial();
			SelectedMat->AddTexture(texture);
		}

		return true;
	}

	bool Application::OnAddModel(AddModelEvent& e)
	{
		if (e.GetModelType() == ModelType::CUBE)
		{
			m_Objects3D.emplace_back(new Cube(GM::Vector3::ZeroVector, GM::Rotator::ZeroRotator, GM::Vector3::UnitVector, m_DefaultMaterial));
			m_SelectedObject3D = m_Objects3D[m_Objects3D.size() - 1];
		}
		else if (e.GetModelType() == ModelType::CUSTOM)
		{
			FileOpenDialog dialog(ResourceType::MODELS);
			dialog.Show();
			
			Ref<Mesh3D> Mesh = Mesh3D::Load(EngineUtil::ToByteString(dialog.GetAbsolutePath()), m_DefaultMaterial);
			Mesh->InitResources();
			m_Objects3D.emplace_back(Mesh);
		}
		// Add more model types once added

		m_SelectedObject3D->bShowDetails = true;
		return true;
	}

	bool Application::OnCameraFOVChanged(class CameraFOVChangedEvent& e)
	{
		CameraController* Controller = e.GetEntity().GetCameraController();
		if (Controller != nullptr)
		{
			Controller->SetFieldOfView(e.GetChangedFOV());
			return true;
		}

		GX_ENGINE_ERROR("Trying to change Camera FOV for a camera without a controller");
		return false;
	}

	bool Application::OnCameraProjectionModeChanged(class CameraProjectionModeChange& e)
	{
		CameraController* Controller = e.GetEntity().GetCameraController();
		if (Controller != nullptr)
		{
			Controller->SetProjectionMode(e.GetNewProjectionMode());
			return true;
		}

		GX_ENGINE_ERROR("Trying to change Projection for a camera without a controller");
		return false;
	}

	bool Application::OnCreateTerrain(CreateTerrainEvent& e)
	{
		m_Terrain.push_back(e.GetTerrain());
		return true;
	}

	bool Application::Exit()
	{
		m_IsRunning = false;
		return true;
	}

#pragma endregion

	Application::~Application()
	{
		GX_ENGINE_INFO("Application: Closing Application.");

		// Release the mesh resources
		for (size_t i = 0; i < m_Objects3D.size(); i++)
			m_Objects3D[i]->ReleaseResources();

		// Release the terrain resources
		for (size_t i = 0; i < m_Terrain.size(); i++)
			m_Terrain[i]->ReleaseResources();

		// TODO: Cleanup all the renderer assets (vao, vbo, ibo, shader, material, textures, etc.) before losing the opengl contex
		// i.e. window destruction
		ParticleManager::Shutdown();
		
		Renderer::Shutdown();

		/* Cleanup the ImGui */
		GraphXGui::Cleanup();

		delete m_Window;

		/* Release resources of subsystems */
		Multithreading::Shutdown();
	}
}