#pragma once

struct GLFWwindow;

namespace GraphX
{
	class GraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* WindowHandle);
		
		bool Init();

	private:
		GLFWwindow* m_WindowHandle;
	};
}