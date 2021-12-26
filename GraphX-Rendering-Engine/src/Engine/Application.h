#pragma once

namespace GraphX
{
	class Window;
	class Mesh2D;
	class Mesh3D;
	class Material;
	class Shader;
	class Texture2D;
	class Skybox;
	class FrameBuffer;
	class Terrain;
	class CameraController;

	class Light;
	class PointLight;
	class DirectionalLight;

	class ParticleManager;

	class Application
	{
	public:
		Application(const char* title, int width, int height);

		/* Application main method */
		void Run();

		void LoadScene();

		/* Returns the window of the engine */
		Window* GetWindow() const { return m_Window; }

		/* Returns the current application instance */
		static Application& Get() { return *s_Instance; }

		~Application();

	private:
		/* Callback for events */
		void OnEvent(class Event& e);

#pragma region eventHandlers

		/* Event Handlers for window events */
		bool OnWindowResize(class WindowResizedEvent& e);

		bool OnWindowMoved(class WindowMovedEvent& e);

		bool OnWindowFocus(class WindowFocusEvent& e);

		bool OnWindowLostFocus(class WindowLostFocusEvent& e);

		bool OnWindowClose(class WindowCloseEvent& e);

		/* Event Handlers for Mouse Input */
		bool OnMouseButtonPressed(class MouseButtonPressedEvent& e);

		bool OnMouseButtonReleased(class MouseButtonReleasedEvent& e);

		bool OnMouseMoved(class MouseMovedEvent& e);

		bool OnMouseScrolled(class MouseScrolledEvent& e);

		/* Event handlers for keyboard Input */
		bool OnKeyPressed(class KeyPressedEvent& e);

		bool OnKeyReleased(class KeyReleasedEvent& e);

		/* Gui Events */
		bool OnAddTexture(class AddTextureEvent& e);

		bool OnAddModel(class AddModelEvent& e);

		bool OnCameraFOVChanged(class CameraFOVChangedEvent& e);

		bool OnCameraProjectionModeChanged(class CameraProjectionModeChange& e);

		bool OnCreateTerrain(class CreateTerrainEvent& e);

		/* Other Events */

		// Exits out of the game loop (or closes the engine)
		bool Exit();

#pragma endregion

		/* Initializes all the components of the application */
		void InitializeApplication();

		/* Updates the scene */
		void Update(float DeltaTime);

		void UpdateCameraUniforms();

		/* Configure the shader for rendering by setting the proper uniforms */
		void ConfigureShaderForRendering(class Shader& shader);

		/* Calculates the shadows */
		void RenderShadowMap();

		/* Renders the scene */
		void RenderScene(bool IsShadowPhase = false);

		/* Separate method for rendering 2D stuff (Only for now - To be separated in to different layers later)*/
		void Render2DScene();

		/* Renders the terrain in the scene */
		void RenderTerrain(bool IsShadowPhase);

		/* Renders the GUI */
		void RenderGui();

		/* All the calculations related to the day night cycle */
		void DayNightCycleCalculations(float DeltaTime);

		/* Mouse pick logic*/
		void PickObject();

		/* Loads any new resources (loaded in b/w frames) in the pool. [This will called from the main thread only] */
		void LoadNewResources();

		/* Callback fn for when a Mesh3D is loaded */
		void OnMesh3DLoad(const Ref<Mesh3D>& LoadedMesh);

	private:
		/* Current instance of the application using the engine */
		static Application* s_Instance;

		/* Application window */
		Window* m_Window;

		/* Title of the application window */
		const char* m_Title;

		/* Whether the application is running or not */
		bool m_IsRunning;

		/* Whether the application is minimised or not */
		bool m_IsMinimised = false;

		/* How long will a day last in the engine (in hours) */
		float m_EngineDayTime;

		/* Collection of all the 2D objects in the scene */
		std::vector<Ref<Mesh2D>> m_Objects2D;

		/* Collection of all the 3D objects in the scene */
		std::vector<Ref<Mesh3D>> m_Objects3D;

		/* All the shaders used in the scene */
		std::vector<Ref<Shader>> m_Shaders;

		/* All the lights in the scene */
		std::vector<Ref<Light>> m_Lights;

		/* All the terrain in the scene */
		std::vector<Ref<Terrain>> m_Terrain;

		/* Current selected 2D object */
		Ref<Mesh2D> m_SelectedObject2D;

		/* Currently selected 3D object */
		Ref<Mesh3D> m_SelectedObject3D;

		/* Main light source for the scene */
		Ref<DirectionalLight> m_SunLight;

		/* Depth Frame buffer for shadow calculation */
		Ref<FrameBuffer> m_ShadowBuffer;

		/* Shader used for rendering to depth buffer */
		Ref<Shader> m_DepthShader;

		/* Main Camera for the application */
		Ref<CameraController> m_CameraController;

		/* SKybox for the day time */
		Ref<Skybox> m_DaySkybox;

		/* Skybox for the night time */
		Ref<Skybox> m_NightSkybox;

		/* Current active skybox */
		Ref<Skybox> m_CurrentSkybox;

		Ref<Shader> m_Shader;

		/* Default Material used by objects */
		Ref<Material> m_DefaultMaterial;

		Ref<PointLight> m_Light;

		Ref<Texture2D> m_DefaultTexture;

		/* 3D Meshes loaded b/w the frames */
		std::deque<Ref<Mesh3D>> m_Loaded3DMeshes;

		/* Mutex used for dealing with asynchronous loading and other stuff for the Mesh3D */
		std::mutex m_Mesh3DMutex;
	};

	// To be defined in GraphX CLIENTS
	Application* CreateApplication();
}