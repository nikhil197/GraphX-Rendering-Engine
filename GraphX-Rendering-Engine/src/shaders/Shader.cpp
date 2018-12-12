#include "pch.h"
#include "Shader.h"

#include "../timer/Timer.h"

namespace engine
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

	void Shader::SetUniform1i(const char* Name, int Val)
	{
		GLCall(glUniform1i(GetLocation(Name), Val));
	}

	void Shader::SetUniform1f(const char* Name, float Val)
	{
		GLCall(glUniform1f(GetLocation(Name), Val));
	}

	void Shader::SetUniform3f(const char* Name, float r, float g, float b)
	{
		GLCall(glUniform3f(GetLocation(Name), r, g, b));
	}

	void Shader::SetUniform3f(const char* Name, const gm::Vector3& Vec)
	{
		GLCall(glUniform3f(GetLocation(Name), Vec.x, Vec.y, Vec.z));
	}

	void Shader::SetUniform4f(const char* Name, float r, float g, float b, float a)
	{
		GLCall(glUniform4f(GetLocation(Name), r, g, b, a));
	}

	void Shader::SetUniform4f(const char* Name, const gm::Vector4& Vec)
	{
		GLCall(glUniform4f(GetLocation(Name), Vec.x, Vec.y, Vec.z, Vec.w));
	}

	void Shader::SetUniformMat3f(const char* Name, const gm::Matrix3& Mat)
	{
		GLCall(glUniformMatrix3fv(GetLocation(Name), 1, GL_TRUE, &Mat[0][0]));
	}

	void Shader::SetUniformMat4f(const char* Name, const gm::Matrix4& Mat)
	{
		GLCall(glUniformMatrix4fv(GetLocation(Name), 1, GL_TRUE, &Mat[0][0]));
	}

	int Shader::GetLocation(const std::string& Name)
	{
		// First check if the uniform location is cached and return it
		if (m_UniformLocations.find(Name) != m_UniformLocations.end())
			return m_UniformLocations[Name];
		else
		{
			// Get the location of the uniform
			int location = glGetUniformLocation(m_RendererID, Name.c_str());
				
			//If the name is invalid
			if (location == -1)
			{
				GX_ENGINE_WARN("Shader: {0} uniform not present in the current bound shader", Name);
			}
			// Cache the location
			else
			{
				m_UniformLocations[Name] = location;
			}

			return location;
		}
	}

	ShaderSource Shader::ParseShaderSource(const std::string& filePath)
	{
		GX_ENGINE_INFO("Parsing Shader source");
		Timer time("Parsing Shader Source");

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
		GX_ENGINE_INFO("Compiling Shaders");
		Timer time("Compiling Shaders");

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

			GX_ENGINE_ERROR("Shader: Failed to compile {0} shader", (type == GL_VERTEX_SHADER) ? "Vertex " : "Fragment ");
			GX_ENGINE_ERROR(infoLog);

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