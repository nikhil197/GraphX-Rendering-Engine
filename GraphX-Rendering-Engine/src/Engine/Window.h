#pragma once

// Forward declaration of needs to be outside the engine namespace since the window is a part of an external library
struct GLFWwindow;

namespace GraphX
{
	class Event;

	/* Properties of the window */
	struct WindowProps
	{
		// Title of the window
		std::string Title;

		// Width and height of the window
		unsigned int Width, Height;

		WindowProps(const std::string& title = "GraphX Engine", unsigned int width = 1920, unsigned int height = 1080)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		/* Constructor */
		Window(const WindowProps& winProps = WindowProps());

		/* Set the clear color */
		void SetClearColor(float r, float g, float b, float a);

		/* Set the event callback for the application */
		void SetEventCallback(const EventCallbackFn& func) { m_Data.EventCallback = func; }

		// Set/Get VSync
		void SetVSync(bool enabled);
		bool IsVSync() const;

		/* Clear the screen before each frame */
		void Clear();

		/* Clears the Depth Buffer */
		void ClearDepthBuffer();

		/* Update the window */
		void OnUpdate();

		/* Resize the window */
		void OnResize();

		/* Returns the underlying native window */
		inline void* GetNativeWindow() const { return m_Window; }

		/* Returns the width of the window */
		inline int GetWidth() const { return m_Data.Width; }

		/* Returns the height of the window */
		inline int GetHeight() const { return m_Data.Height; }

		/* Destruct the window */
		~Window();

	private:
		/* Initialise the window */
		void Init(const WindowProps& props);

		void OnEvent(Event& event);

	private:
		/* GLFW Window */
		GLFWwindow* m_Window;

		/* Graphics Context to render on */
		class GraphicsContext* m_Context;

		struct WindowData
		{
			// Title of the window
			std::string Title;

			// Width and height of the window
			unsigned int Width, Height;

			// Function pointer to the Application Event callback
			EventCallbackFn EventCallback;

			// To enable VSync or not
			bool Vsync;
		};

		// Data passed to GLFW
		WindowData m_Data;
	};
}