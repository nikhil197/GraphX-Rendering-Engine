#pragma once

// Forward declaration of needs to be outside the engine namespace since the window is a part of an external library
struct GLFWwindow;

namespace engine
{
	class Window
	{
	private:
		/* Title of the window */
		std::string m_Title;

		/* Width and height of the window */
		int m_Width, m_Height;

		/* GLFW Window */
		GLFWwindow* m_Window;

	private:
		/* Initialise the window */
		bool Init();

	public:
		/* Constructor */
		Window(std::string title, int width, int height);

		/* Set the clear color */
		void SetClearColor(float r, float g, float b, float a);

		/* Returns whether the window has been closed */
		bool IsClosed() const;

		/* Resize the window */
		void Resize();

		/* Clear the screen before each frame */
		void Clear();

		/* Update the window */
		void Update();

		/* Returns the width of the window */
		inline int GetWidth() const { return m_Width; }

		/* Returns the height of the window */
		inline int GetHeight() const { return m_Height; }

		/* Destruct the window */
		~Window();
	};
}