#pragma once

namespace engine
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

		/* Collection of all the 2D objects in the scene */
		std::vector<class Mesh2D*> m_Objects2D;

		/* Collection of all the 3D objects in the scene */
		std::vector<class Mesh3D*> m_Objects3D;

		/* Current selected 2D object */
		class Mesh2D* m_SelectedObject2D;

		/* Currently selected 3D object */
		class Mesh3D* m_SelectedObject3D;

		class Shader* m_Shader;

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

#pragma endregion

	public:
		Application(std::string& title, int width, int height);

		/* Initializes the OpenGL elements, Returns whether initialization was successful */
		bool InitializeOpenGL();

		/* Application main method */
		void Run();

		~Application();
	};
}