#include "pch.h"
#include "ErrorHandler.h"

namespace engine
{
	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLLogCall(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError())
		{
			GX_ENGINE_ERROR("[OpenGL Error] : ( {0} ), fn {1}, file {2}, line {3}", error, function, file, line);
			return false;
		}

		return true;
	}

	void GlfwErrorCallback(int error, const char* description)
	{
		GX_ENGINE_ERROR("[GFLW Error] : {0}, {1}", error, description);
	}
}