#include "pch.h"
#include "GraphicsContext.h"
#include "GL/glew.h"

#include "GLFW\glfw3.h"

namespace GraphX
{
	const char* GetDebugMessageSource(GLenum source)
	{
		switch (source)
		{
			case GL_DEBUG_SOURCE_API:				return "OpenGL API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "Window System";
			case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "Shader Compiler";
			case GL_DEBUG_SOURCE_THIRD_PARTY:		return "3rd Party";
			case GL_DEBUG_SOURCE_APPLICATION:		return "Application";
			case GL_DEBUG_SOURCE_OTHER:				return "Other";
		}

		GX_ASSERT(false, "Unknown Debug Source");
		return "";
	}

	const char* GetDebugMessageSeverity(GLenum severity)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:			return "High";
			case GL_DEBUG_SEVERITY_MEDIUM:			return "Medium";
			case GL_DEBUG_SEVERITY_LOW:				return "Low";
			case GL_DEBUG_SEVERITY_NOTIFICATION:	return "Notification";
		}

		GX_ASSERT(false, "Unkown Debug Severity");
		return "";
	}

	const char* GetDebugMessageType(GLenum type)
	{
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:				return "Error";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "Undefined Behavior";
			case GL_DEBUG_TYPE_PORTABILITY:			return "Portability";
			case GL_DEBUG_TYPE_PERFORMANCE:			return "Performance";
			case GL_DEBUG_TYPE_OTHER:				return "Other";
			case GL_DEBUG_TYPE_MARKER:				return "MARKER";
			case GL_DEBUG_TYPE_PUSH_GROUP:			return "Push Group";
			case GL_DEBUG_TYPE_POP_GROUP:			return "Pop Group";
		}

		GX_ASSERT(false, "Unknown Message Type");
		return "";
	}

	static void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		const char* src = GetDebugMessageSource(source);
		const char* severe = GetDebugMessageSeverity(severity);
		const char* msgType = GetDebugMessageType(type);

		if (type == GL_DEBUG_TYPE_ERROR || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
		{
			GX_ENGINE_ERROR("[OpenGL Error] : ( {0} ) -> {1} \n   Source: {2} \n   Type: {3} \n   Severity: {4}", id, message, src, msgType, severe);
			GX_ASSERT(false, "Error");
		}
		else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || type == GL_DEBUG_TYPE_PERFORMANCE)
		{
			GX_ENGINE_WARN("[OpenGL Warning] : ( {0} ) -> {1} \n   Source: {2} \n   Type: {3} \n   Severity: {4}", id, message, src, msgType, severe);
		}
		else
		{
			GX_ENGINE_INFO("[OpenGL Debug Message] : ( {0} ) -> {1} \n   Source: {2} \n   Type: {3} \n   Severity: {4}", id, message, src, msgType, severe);
		}
	}

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

		// Enable Debugging
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, 0);

		// To enable the depth test
		glEnable(GL_DEPTH_TEST);

		// Enable back face culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// To enable blending
		//GLCall(glEnable(GL_BLEND));

		// Blend function
		// src is the alpha of the current pixel
		// dest is the alpha of that is already in the buffer
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Blend Equation
		// Add the src and dest values to get the result (can be changed to subtract, inverse, etc.)
		glBlendEquation(GL_FUNC_ADD);

		return true;
	}
}