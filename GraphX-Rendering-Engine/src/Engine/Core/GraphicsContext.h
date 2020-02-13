#pragma once

struct GLFWwindow;

namespace GraphX
{
	class GraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* WindowHandle);
		
		/* Initialise the Context for the window handle */
		void Init();

	private:
		/* A handle to the GLFW Window */
		GLFWwindow* m_WindowHandle;
	};
}