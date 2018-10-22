#include "Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "../ErrorHandler.h"

namespace engine
{
	namespace shaders
	{
		Shader::Shader(const std::string& filePath)
			:m_FilePath(filePath), m_RendererID(0)
		{
			ShaderSource source = ParseShaderSource(filePath);
			m_RendererID = CreateShader(source.VertexShaderSource, source.FragmentShaderSource);
		}

		Shader::~Shader()
		{
			GLCall(glDeleteProgram(m_RendererID));
		}

		void Shader::Bind() const
		{
			GLCall(glUseProgram(m_RendererID));
		}

		void Shader::UnBind() const
		{
			GLCall(glUseProgram(0));
		}

		void Shader::SetUniform4f(float r, float g, float b, float a, const char* Name)
		{
			int location = glGetUniformLocation(m_RendererID, Name);
			glUniform4f(location, r, g, b, a);
		}

		ShaderSource Shader::ParseShaderSource(const std::string& filePath)
		{
			enum class ShaderType
			{
				NONE = -1, VERTEX = 0, FRAGMENT = 1
			};

			std::stringstream shaderStrings[2];

			std::fstream stream(filePath);
			std::string line;

			ShaderType type = ShaderType::NONE;

			while (getline(stream, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = ShaderType::VERTEX;
					else if (line.find("fragment") != std::string::npos)
						type = ShaderType::FRAGMENT;
				}
				else
					shaderStrings[(int)type] << line << "\n";
			}

			return { shaderStrings[0].str(), shaderStrings[1].str() };
		}

		unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
		{
			int shaderID = glCreateShader(type);
			const char* src = source.c_str();
			GLCall(glShaderSource(shaderID, 1, &src, nullptr));
			GLCall(glCompileShader(shaderID));

			int result;
			GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result));

			if (result == GL_FALSE)
			{
				int length;
				GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));

				char* infoLog = (char*)alloca(length * sizeof(char));
				GLCall(glGetShaderInfoLog(shaderID, length, &length, infoLog));

				std::cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "Vertex " : "Fragment ") << " shader" << std::endl;
				std::cout << infoLog << std::endl;

				GLCall(glDeleteShader(shaderID));
				return 0;
			}

			return shaderID;
		}

		unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
		{
			int programID = glCreateProgram();
			unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
			unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

			GLCall(glAttachShader(programID, vs));
			GLCall(glAttachShader(programID, fs));
			GLCall(glLinkProgram(programID));
			GLCall(glValidateProgram(programID));

			GLCall(glDeleteShader(vs));
			GLCall(glDeleteShader(fs));

			return programID;
		}
	}
}