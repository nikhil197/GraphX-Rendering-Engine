#pragma once

#define GX_ASSERT(x, ...) { if(!(x)) { GX_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define GLCall(x) GLClearError();\
	x;\
	GX_ASSERT(GLLogCall(#x, __FILE__, __LINE__), "")

#include "GL/glew.h"

namespace GraphX
{
	/* Clear all the OpenGL errors */
	void GLClearError();

	/* Log an OpenGL call */
	bool GLLogCall(const char* function, const char* file, int line);
}