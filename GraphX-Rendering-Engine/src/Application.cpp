#include "pch.h"

#include "Application.h"

#include "VertexArray.h"
#include "Shaders/Shader.h"

/* Buffers */
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/FrameBuffer.h"

#include "Model/Mesh/Vertex.h"
#include "Textures/Texture.h"

/* Renderer */
#include "Renderer/SimpleRenderer.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/Renderer2D.h"

/* Entities */
#include "Entities/Lights/PointLight.h"
#include "Entities/Lights/DirectionalLight.h"
#include "Entities/Camera.h"
#include "Entities/Skybox.h"
#include "Entities/Terrain.h"
#include "Entities/Particles/Particle.h"
#include "Entities/Particles/ParticleManager.h"
#include "Entities/Particles/ParticleSystem.h"

#include "Window.h"
#include "Timer/Clock.h"
#include "Gui/GraphXGui.h"

/* Events */
#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/GUIEvent.h"

/* Input */
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

/* Model */
#include "Model/ModelTypes.h"
#include "Model/Model3D.h"
#include "Model/Mesh/Mesh2D.h"
#include "Model/Mesh/Mesh3D.h"
#include "model/Cube.h"

/* Utils */
#include "Utilities/EngineUtil.h"
#include "Utilities/FileOpenDialog.h"

namespace engine
{
	using namespace gm;

	Application::Application(std::string& title, int width, int height)
		: m_Window(nullptr), m_Title(title), m_IsRunning(true), m_EngineDayTime(0.1f), m_SelectedObject2D(nullptr), m_SelectedObject3D(nullptr), m_SunLight(nullptr), m_ShadowBuffer(nullptr), m_DepthShader(nullptr), m_Camera(nullptr), m_DaySkybox(nullptr), m_NightSkybox(nullptr), m_CurrentSkybox(nullptr), m_Renderer3D(nullptr), m_Renderer(nullptr), m_ParticlesManager(nullptr), m_Shader(nullptr), m_Light(nullptr), m_DefaultTexture(nullptr)
	{
		// Initialise the clock and the logging, and the input devices
		Log::Init();
		Clock::Init();
		Mouse::Init();
		Keyboard::Init();

		m_Window = new Window(m_Title, width, height);

		bool success = InitializeOpenGL();
		if (!success)
		{
			GX_ENGINE_ERROR("Application: Failed to intialize OpenGL");
			ASSERT(success);
		}

		InitializeApplication();
	}

	bool Application::InitializeOpenGL()
	{
		Timer timer("Initializing OpenGL");

		// Initialise GLEW
		if (glewInit() != GLEW_OK)
		{
			GX_ENGINE_ERROR("Application: Error while initializing glew");
			return false;
		}

		// Print the gl version
		GX_ENGINE_INFO("OpenGL Info:");
		GX_ENGINE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
		GX_ENGINE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		GX_ENGINE_INFO("	Version: {0}", glGetString(GL_VERSION));

		// To enable the depth test
		GLCall(glEnable(GL_DEPTH_TEST));

		// Enable back face culling
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_BACK));

		// To enable blending
		//GLCall(glEnable(GL_BLEND));

		// Blend function
		// src is the alpha of the current pixel
		// dest is the alpha of that is already in the buffer
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Blend Equation
		// Add the src and dest values to get the result (can be changed to subtract, inverse, etc.)
		GLCall(glBlendEquation(GL_FUNC_ADD));

		return true;
	}

	void Application::InitializeApplication()
	{
		// Set the event callback with the window
		m_Window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		m_Camera = new Camera(gm::Vector3(0.0f, 0.0f, 3.0f), gm::Vector3::ZeroVector, gm::Vector3::YAxis, (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), GX_ENGINE_NEAR_PLANE, GX_ENGINE_FAR_PLANE);

		std::vector<std::string> SkyboxNames = { "right.png", "left.png" , "top.png" , "bottom.png" , "front.png" , "back.png" };
		m_DaySkybox  = new Skybox("res/Shaders/Skybox.shader", "res/Textures/Skybox/Day/", SkyboxNames, *m_Camera, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_NightSkybox = new Skybox("res/Shaders/Skybox.shader", "res/Textures/Skybox/Night/", SkyboxNames, *m_Camera, Vector4(0.5f, 0.5f, 0.5f, 1.0f), 0.0f, 0, 0.f);

		m_CurrentSkybox = m_NightSkybox;

		m_SunLight = new DirectionalLight(gm::Vector4::UnitVector, gm::Vector3(-1.0f, -1.0f, 1.0f));
		m_Lights.emplace_back(m_SunLight);

		m_Light = new PointLight(Vector3(0, 50.0f, 50.0f), Vector4(1, 1, 1, 1));
		m_Lights.emplace_back(m_Light);

		m_ShadowBuffer = new FrameBuffer(m_Window->GetWidth(), m_Window->GetHeight(), FramebufferType::GX_FRAME_DEPTH);
		m_DepthShader = new Shader("res/Shaders/Depth.shader");

		m_Renderer3D = new Renderer3D();
		m_Renderer = new SimpleRenderer();

		m_ParticlesManager = new ParticleManager(*m_Camera);

		m_DefaultTexture  = new Texture("res/Textures/stone.jpg");
	}

	void Application::Run()
	{
		// Basic Lighting Shader 
		m_Shader = new Shader("res/Shaders/BasicLightingShader.shader");
		m_Shaders.push_back(m_Shader);
		
		m_Shader->Bind();
		m_Shader->SetUniform1f("u_AmbientStrength", 0.1f);
		m_Shader->SetUniform1f("u_Shininess", 256.0f);
		m_Shader->SetUniform1f("u_Reflectivity", 1.0f);
		
		m_Shader->SetUniform3f("u_LightPos", m_Light->Position);
		m_Shader->SetUniform4f("u_LightColor", m_Light->Color);

		// For the purpose of fps count
		int times = 0;
		float then = Clock::GetClock()->GetEngineTime();

		std::vector<const Texture*> textures(0);
		textures.push_back(m_DefaultTexture);

		Cube *cube = new Cube(gm::Vector3(-10.0f, 10.0f, -5.0f), gm::Vector3::ZeroVector, gm::Vector3::UnitVector, *m_Shader, textures);
		m_Objects3D.emplace_back(cube);
		cube->bShowDetails = true;

		Terrain *ter = new Terrain(250, 250, 2.0f, {"res/Textures/Terrain/Grass.png", "res/Textures/Terrain/GrassFlowers.png", "res/Textures/Terrain/Mud.png", "res/Textures/Terrain/Path.png"}, "res/Textures/Terrain/BlendMap.png", Vector3(-249.0f, 10.0f, 249.0f), Vector2(1.0f, 1.0f));
		m_Shaders.emplace_back(ter->GetShader());
		m_Terrain.emplace_back(ter);

		// Load Trees
		Model3D TreeModel("res/Models/tree.obj", *m_Shader);
		Texture TreeTex("res/Textures/tree.png");
		TreeModel.GetMeshes().at(0)->AddTexture(&TreeTex);
		Mesh3D* TreeMesh = TreeModel.GetMeshes().at(0);
		TreeMesh->Scale = 2.5f * Vector3::UnitVector;
		unsigned int NumTree = 100;
		for (unsigned int i = 0; i < NumTree; i++)
		{
			Vector3 Position((2 * (float)EngineUtil::GetRandomValue() - 1) * ter->GetWidth() / 2, 0.0f, (2 * (float)EngineUtil::GetRandomValue() - 1) * ter->GetDepth() / 2);
			TreeMesh->Position = Position;
			m_Objects3D.emplace_back(new Mesh3D(*TreeMesh));
		}

		Model3D LowPolyTreeModel("res/Models/lowPolyTree.obj", *m_Shader);
		Texture LowPolyTreeTex("res/Textures/lowPolyTree.png");
		LowPolyTreeModel.GetMeshes().at(0)->AddTexture(&LowPolyTreeTex);
		Mesh3D* LowPolyTreeMesh = LowPolyTreeModel.GetMeshes().at(0);
		LowPolyTreeMesh->Scale = Vector3::UnitVector;
		NumTree = 10;
		for (unsigned int i = 0; i < NumTree; i++)
		{
			Vector3 Position((2 * (float)EngineUtil::GetRandomValue() - 1) * ter->GetWidth() / 2, 0.0f, (2 * (float)EngineUtil::GetRandomValue() - 1) * ter->GetDepth() / 2);
			LowPolyTreeMesh->Position = Position;
			m_Objects3D.emplace_back(new Mesh3D(*LowPolyTreeMesh));
		}

		// Load Stall
		Model3D StallModel("res/Models/stall.obj", *m_Shader);
		Texture StallTex("res/Textures/stallTexture.png");
		StallModel.GetMeshes().at(0)->AddTexture(&StallTex);
		StallModel.GetMeshes().at(0)->Position = Vector3(75.0f, 0.0f, -100.0f);
		m_Objects3D.emplace_back(StallModel.GetMeshes().at(0));

		m_Shader->UnBind();

		ter->GetShader()->Bind();
		ter->GetShader()->SetUniform1f("u_AmbientStrength", 0.01f);
		ter->GetShader()->SetUniform1f("u_Shininess", 256.0f);
		ter->GetShader()->SetUniform1f("u_Reflectivity", 1.0f);

		Texture particleTex("res/Textures/Particles/particleAtlas.png", false, 4);
		ParticleSystem particleSys(*m_ParticlesManager, particleTex, 50.0f, 2.0f, 0.5f, 2.0f, 1.0f, 0.5f, 0.4f, 0.5f, 1.0f);

		// Draw while the window doesn't close
		while (m_IsRunning)
		{
			for (unsigned int i = 0; i < m_Objects3D.size(); i++)
				m_Renderer3D->Submit(m_Objects3D[i]);

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
			}

			if (GX_ENABLE_PARTICLE_EFFECTS)
			{
				particleSys.SpawnParticles(gm::Vector3::ZeroVector, DeltaTime);
			}

			// Update all the elements of the scene
			Update(DeltaTime);

			// Calculate the shadow maps
			if(GX_ENABLE_SHADOWS)
				RenderShadowMap();

			/****** Normally render the scene *****/
			// Clear the window 
			m_Window->Clear();

			// Draw the debug quad to show the depth map
			//RenderShadowDebugQuad();

			RenderSkybox();

			// Bind the shader and draw the objects
			m_Shader->Bind();
			m_ShadowBuffer->BindDepthMap(GX_ENGINE_SHADOW_MAP_TEXTURE_SLOT);
			ConfigureShaderForRendering(*m_Shader);

			RenderScene();

			m_ParticlesManager->RenderParticles();

			RenderGui();

			//Update the mouse
			Mouse::GetMouse()->Update();

			//Poll events and swap buffers
			m_Window->OnUpdate();
		}
	}

	void Application::Update(float DeltaTime)
	{
		// Update the Gui
		GraphXGui::Update();

		// Update the camera
		m_Camera->Update(DeltaTime);
		
		// Update the lights
		for (unsigned int i = 0; i < m_Lights.size(); i++)
			m_Lights[i]->Update(DeltaTime);

		// Update the meshes
		for (unsigned int i = 0; i < m_Objects2D.size(); i++)
			m_Objects2D[i]->Update(DeltaTime);
		
		for (unsigned int i = 0; i < m_Objects3D.size(); i++)
			m_Objects3D[i]->Update(DeltaTime);

		for (unsigned int i = 0; i < m_Terrain.size(); i++)
			m_Terrain[i]->Update(DeltaTime);

		m_ParticlesManager->Update(DeltaTime);

		DayNightCycleCalculations(DeltaTime);

		m_CurrentSkybox->Update(DeltaTime);

		if (m_Camera->IsRenderStateDirty())
		{
			for (unsigned int i = 0; i < m_Shaders.size(); i++)
			{
				Shader* shader = m_Shaders.at(i);
				if (!shader)
				{
					m_Shaders.erase(m_Shaders.begin() + i);		// TODO : Fix Memory leak
					continue;
				}
				shader->Bind();
				shader->SetUniform3f("u_CameraPos", m_Camera->CameraPosition);
				shader->SetUniformMat4f("u_ProjectionView", m_Camera->GetProjectionViewMatrix());
			}

			// Set the state back to rendered
			m_Camera->SetRenderStateDirty(false);
		}
	}

	void Application::RenderShadowMap()
	{
		// Render the shadow maps
		m_DepthShader->Bind();
		m_DepthShader->SetUniformMat4f("u_LightSpaceMatrix", m_SunLight->GetShadowInfo()->LightViewProjMat);
		m_ShadowBuffer->Bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		
		RenderScene(true);

		m_ShadowBuffer->UnBind();
	}

	void Application::RenderSkybox()
	{
		// Render the sky box
		m_CurrentSkybox->Enable();
		m_Renderer->Draw(m_CurrentSkybox->GetIBO());
		m_CurrentSkybox->Disable();
	}

	void Application::RenderScene(bool IsShadowPhase)
	{
		if (IsShadowPhase)
		{
			m_Renderer3D->Render(*m_DepthShader);
		}
		else
			m_Renderer3D->Render();

		RenderTerrain();
	}

	void Application::RenderTerrain()
	{
		for (unsigned int i = 0; i < m_Terrain.size(); i++)
		{
			Terrain* terrain = m_Terrain[i];

			// Configure the terrain shaders
			terrain->Enable();
			Shader& shader = *terrain->GetShader();
			ConfigureShaderForRendering(shader);

			// Set the transformation matrix
			gm::Matrix4 Model = terrain->GetMesh().GetModelMatrix();
			shader.SetUniformMat4f("u_Model", Model);

			// Render the Terrain
			m_Renderer->Draw(*terrain->GetMesh().GetIBO());

			terrain->Disable();
		}
	}

	void Application::RenderGui()
	{
		GraphXGui::DetailsWindow(*m_Objects3D[0]);
		if (m_SelectedObject3D != nullptr)
			GraphXGui::DetailsWindow(*m_SelectedObject3D, "Selected Object");

		GraphXGui::LightProperties(*m_Light);
		GraphXGui::CameraProperties(*m_Camera);
		GraphXGui::Models();
		if (m_Terrain.size() > 0 && m_Terrain[0] != nullptr)
		{
			GraphXGui::TerrainDetails(*m_Terrain[0]);
		}
		GraphXGui::GlobalSettings(*m_CurrentSkybox, m_EngineDayTime, m_SunLight->Intensity, GX_ENABLE_PARTICLE_EFFECTS);
		GraphXGui::Render();
	}

	void Application::RenderShadowDebugQuad()
	{
		static std::vector<Vertex2D> quadVertices = {
			{ Vector2(-0.5f, -0.5f), Vector2(0.0f, 0.0f) },	//0
			{ Vector2( 0.5f, -0.5f), Vector2(1.0f, 0.0f) },	//1
			{ Vector2( 0.5f,  0.5f), Vector2(1.0f, 1.0f) },	//2
			{ Vector2(-0.5f,  0.5f), Vector2(0.0f, 1.0f) }	//3
		};

		static std::vector<unsigned int> quadIndices = {
			0, 1, 2,
			0, 2, 3
		};

		static Shader shader("res/shaders/Basic.shader");

		static Mesh2D QuadMesh(gm::Vector3::ZeroVector, gm::Vector3::ZeroVector, gm::Vector2::UnitVector, shader, {}, quadVertices, quadIndices, Vector4::ZeroVector, -1.0f, -1.0f);

		static Renderer2D renderer;

		shader.Bind();
		m_ShadowBuffer->BindDepthMap();
		shader.SetUniform1i("u_Tex", 0);
		renderer.Submit(QuadMesh);
		renderer.Render();
		shader.UnBind();
	}

	void Application::ConfigureShaderForRendering(Shader& shader)
	{
		shader.SetUniform1i("u_ShadowMap", GX_ENGINE_SHADOW_MAP_TEXTURE_SLOT);
		shader.SetUniform3f("u_LightPos", m_Light->Position);
		shader.SetUniform4f("u_LightColor", m_Light->Color);

		if(GX_ENABLE_SHADOWS)
			shader.SetUniformMat4f("u_LightSpaceMatrix", m_SunLight->GetShadowInfo()->LightViewProjMat);

		m_SunLight->Enable(shader, "u_LightSource");
	}

	void Application::OnEvent(Event& e)
	{
		bool handled = false;
		EventDispatcher dispatcher(e);

		if (e.IsInCategory(GX_EVENT_CATEGORY_WINDOW))
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
		else if (e.IsInCategory(GX_EVENT_CATEGORY_KEYBOARD))
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
		else if (e.IsInCategory(GX_EVENT_CATEGORY_MOUSE))
		{
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
		else if (e.IsInCategory(GX_EVENT_CATEGORY_GUI))
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

	void Application::DayNightCycleCalculations(float DeltaTime)
	{
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
			m_SunLight->Color = gm::Vector4(0.5f, 0.5f, 0.0f, 1.0f);
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
			m_SunLight->Color = gm::Vector4::UnitVector;
			m_CurrentSkybox->BlendFactor = 0.2f;
		}

		if (TimeOfDay > 0.0f)
		{
			m_CurrentSkybox->BlendFactor *= (TimeOfDay / 3.0f);
		}

		float angle = DeltaTime * 25.0f / (m_EngineDayTime * 10.0f);
		gm::Rotation rotation(angle, gm::Vector3::YAxis);
		m_SunLight->Direction = gm::Vector3(rotation * gm::Vector4(m_SunLight->Direction, 1.0f));
	}

#pragma region eventHandlers

	bool Application::OnWindowResize(WindowResizedEvent& e)
	{
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
		Texture *texture = new Texture(TexName);

		if (m_SelectedObject3D)
		{
			m_SelectedObject3D->AddTexture(texture);
		}
		else if (m_SelectedObject2D)
		{
			m_SelectedObject2D->AddTexture(texture);
		}

		return true;
	}

	bool Application::OnAddModel(AddModelEvent& e)
	{
		if (e.GetModelType() == ModelType::CUBE)
		{
			m_Objects3D.emplace_back(new Cube(gm::Vector3::ZeroVector, gm::Vector3::ZeroVector, gm::Vector3::UnitVector, *m_Shader, {}));
			m_SelectedObject3D = m_Objects3D[m_Objects3D.size() - 1];
		}
		else if (e.GetModelType() == ModelType::CUSTOM)
		{
			FileOpenDialog dialog(ResourceType::MODELS);
			dialog.Show();
			
			Model3D *model = new Model3D(EngineUtil::ToByteString(dialog.GetAbsolutePath()), *m_Shader);
			std::vector<Mesh3D*> meshes = model->GetMeshes();
			
			for(unsigned int i = 0; i < meshes.size(); i++)
				m_Objects3D.emplace_back(meshes[i]);

			m_SelectedObject3D = m_Objects3D[m_Objects3D.size() - 1];
		}
		// Add more model types once added

		m_SelectedObject3D->bShowDetails = true;
		return true;
	}

	bool Application::OnCameraFOVChanged(class CameraFOVChangedEvent& e)
	{
		e.GetEntity().SetNewFOV(e.GetChangedFOV());
		return true;
	}

	bool Application::OnCameraProjectionModeChanged(class CameraProjectionModeChange& e)
	{
		e.GetEntity().SetNewProjectionMode(e.GetNewProjectionMode());
		return true;
	}

	bool Application::OnCreateTerrain(CreateTerrainEvent& e)
	{
		m_Terrain.push_back(e.GetTerrain());
		return true;
	}

#pragma endregion

	Application::~Application()
	{
		GX_ENGINE_INFO("Application: Closing Application.");

		delete m_DaySkybox;
		delete m_NightSkybox;
		delete m_ShadowBuffer;
		delete m_DepthShader;

		for (unsigned int i = 0; i < m_Lights.size(); i++)
			delete m_Lights[i];

		for (unsigned int i = 0; i < m_Terrain.size(); i++)
			delete m_Terrain[i];

		for (unsigned int i = 0; i < m_Objects2D.size(); i++)
			delete m_Objects2D[i];

		for (unsigned int i = 0; i < m_Objects3D.size(); i++)
			delete m_Objects3D[i];

		for (unsigned int i = 0; i < m_Shaders.size(); i++)
			delete m_Shaders[i];

		delete m_Renderer3D;
		delete m_Renderer;
		delete m_ParticlesManager;

		delete m_Camera;
		delete m_Window;
	}
}