#pragma once

#include <string>

namespace engine
{
	class GLFWwindow;

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

		/* Destruct the window */
		~Window();
	};
}