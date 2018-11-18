#include "ErrorHandler.h"

#include <iostream>

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
			std::cout << "[OpenGL Error] ( " << error << " )" << function << " " << file << " " << line << std::endl;
			return false;
		}

		return true;
	}

	void GlfwErrorCallback(int error, const char* description)
	{
		std::cout << "[GFLW Error] : " << error << ", " << description << std::endl;
	}
}