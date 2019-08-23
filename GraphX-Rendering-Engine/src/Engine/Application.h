#pragma once

namespace GraphX
{
	class Window;

	class Application
	{
	private:
		/* Application window */
		Window* m_Window;

		/* Title of the application window */
		std::string m_Title;

		/* Whether the application is running or not */
		bool m_IsRunning;

		/* How long will a day last in the engine (in hours) */
		float m_EngineDayTime;

		/* Collection of all the 2D objects in the scene */
		std::vector<class Mesh2D*> m_Objects2D;

		/* Collection of all the 3D objects in the scene */
		std::vector<class Mesh3D*> m_Objects3D;

		/* All the shaders used in the scene */
		std::vector<class Shader*> m_Shaders;

		/* All the lights in the scene */
		std::vector<class Light*> m_Lights;

		/* All the terrain in the scene */
		std::vector<class Terrain*> m_Terrain;

		/* Current selected 2D object */
		class Mesh2D* m_SelectedObject2D;

		/* Currently selected 3D object */
		class Mesh3D* m_SelectedObject3D;

		/* Main light source for the scene */
		class DirectionalLight* m_SunLight;

		/* Depth Frame buffer for shadow calculation */
		class FrameBuffer* m_ShadowBuffer;

		/* Shader used for rendering to depth buffer */
		class Shader* m_DepthShader;

		/* Main Camera for the application */
		class Camera* m_Camera;

		/* SKybox for the day time */
		class Skybox* m_DaySkybox;

		/* Skybox for the night time */
		class Skybox* m_NightSkybox;

		/* Current active skybox */
		class Skybox* m_CurrentSkybox;

		/* To handle all the particles generated */
		class ParticleManager* m_ParticlesManager;

		class Shader* m_Shader;

		class PointLight* m_Light;

		class Texture* m_DefaultTexture;

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

#pragma endregion

		/* Initializes all the components of the application */
		void InitializeApplication();

		/* Updates the scene */
		void Update(float DeltaTime);

		/* Configure the shader for rendering by setting the proper uniforms */
		void ConfigureShaderForRendering(class Shader& shader);

		/* Calculates the shadows */
		void RenderShadowMap();

		/* Renderes the skybox */
		void RenderSkybox();

		/* Renders the scene */
		void RenderScene(bool IsShadowPhase = false);

		/* Renders the terrain in the scene */
		void RenderTerrain(bool IsShadowPhase);

		/* Renders the GUI */
		void RenderGui();

		/* Renders a quad for debugging shadow map */
		void RenderShadowDebugQuad();

		/* All the calculations related to the day night cycle */
		void DayNightCycleCalculations(float DeltaTime);

	public:
		Application(std::string& title, int width, int height);

		/* Application main method */
		void Run();

		~Application();
	};
}