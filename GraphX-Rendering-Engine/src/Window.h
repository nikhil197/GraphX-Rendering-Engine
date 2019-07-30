#pragma once

// Forward declaration of needs to be outside the engine namespace since the window is a part of an external library
struct GLFWwindow;

namespace GraphX
{
	class Event;

	class Window
	{
	private:
		/* Title of the window */
		std::string m_Title;

		/* Width and height of the window */
		int m_Width, m_Height;

		/* Whether the window is closed */
		bool m_Closed;

		/* GLFW Window */
		GLFWwindow* m_Window;

		/* Function pointer to the Application Event callback*/
		std::function<void(Event&)> m_EventCallback;

	private:
		/* Initialise the window */
		bool Init();

		// To handle all the events
		void OnEvent(Event& e);

#pragma region callbacks

		/************* Window Callbacks **************/
		void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

		void WindowPositionCallback(GLFWwindow* window, int xpos, int ypos);

		void WindowFocusCallback(GLFWwindow* window, int focused);

		void WindowCloseCallback(GLFWwindow* window);

		/************* Input Callbacks **************/
		void KeyCallback(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);

		void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

		void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#pragma endregion

	public:
		/* Constructor */
		Window(std::string title, int width, int height);

		/* Set the clear color */
		void SetClearColor(float r, float g, float b, float a);

		/* Set the event callback for the application */
		void SetEventCallback(const std::function<void(Event&)>& func);

		/* Returns whether the window has been closed */
		inline bool IsClosed() const { return m_Closed; }

		/* Clear the screen before each frame */
		void Clear();

		/* Update the window */
		void OnUpdate();

		/* Resize the window */
		void OnResize();

		/* Returns the width of the window */
		inline int GetWidth() const { return m_Width; }

		/* Returns the height of the window */
		inline int GetHeight() const { return m_Height; }

		/* Destruct the window */
		~Window();
	};
}