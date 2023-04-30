#pragma once

struct GLFWwindow;

namespace GraphX
{
	class GraphicsContext
	{
	public:
		// For OpenGL: https://www.khronos.org/opengl/wiki/OpenGL_Context#Context_information_queries
		struct ContextInfo
		{
			int MajorVersion;
			int MinorVersion;
			std::string Vendor;
			std::string RendererName;
		};

	public:
		GraphicsContext(GLFWwindow* WindowHandle);
		
		/* Initialise the Context for the window handle */
		void Init();

		inline const ContextInfo& GetContextInfo() const { return m_Info; }

	private:
		/* A handle to the GLFW Window */
		GLFWwindow* m_WindowHandle;

		/* Information about the Graphics Context */
		ContextInfo m_Info;
	};
}