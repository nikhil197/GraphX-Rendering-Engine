#pragma once
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#include "GL/glew.h"

namespace GraphX
{
	/* Clear all the OpenGL errors */
	void GLClearError();

	/* Log an OpenGL call */
	bool GLLogCall(const char* function, const char* file, int line);

	/* Error callback for glfw */
	void GlfwErrorCallback(int error, const char* decription);
}