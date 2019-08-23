#include "pch.h"
#include "GraphicsContext.h"

#include "GLFW\glfw3.h"

namespace GraphX
{
	GraphicsContext::GraphicsContext(GLFWwindow* WindowHandle)
		: m_WindowHandle(WindowHandle)
	{
	}

	bool GraphicsContext::Init()
	{
		GX_ENGINE_INFO("Initializing OpenGL");
		Timer timer("Initializing OpenGL");

		glfwMakeContextCurrent(m_WindowHandle);

		// Initialise GLEW
		if (glewInit() != GLEW_OK)
		{
			GX_ENGINE_ERROR("Error while initializing glew");
			return false;
		}

		// Print the gl version
		GX_ENGINE_INFO("OpenGL Info:");
		GX_ENGINE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
		GX_ENGINE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		GX_ENGINE_INFO("	Version: {0}", glGetString(GL_VERSION));

		// To enable the depth test
		GLCall(glEnable(GL_DEPTH_TEST));

		// Enable back face culling
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_BACK));

		// To enable blending
		//GLCall(glEnable(GL_BLEND));

		// Blend function
		// src is the alpha of the current pixel
		// dest is the alpha of that is already in the buffer
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Blend Equation
		// Add the src and dest values to get the result (can be changed to subtract, inverse, etc.)
		GLCall(glBlendEquation(GL_FUNC_ADD));

		return true;
	}
}