#include "pch.h"
#include "Shader.h"

#include "../Timer/Timer.h"

namespace engine
{
	Shader::Shader(const std::string& filePath)
		:m_FileName(std::string()), m_RendererID(0)
	{
		ParseFileName(filePath);
		ShaderSource source = ParseShaderSource(filePath);
		
		if (source.FragmentShaderSource.length() > 0 && source.VertexShaderSource.length() > 0)
			m_RendererID = CreateShader(source.VertexShaderSource, source.FragmentShaderSource);
		else
			GX_ENGINE_ERROR("Error while creating the shader {0}, Source not found", m_FileName);
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_RendererID));
	}

	void Shader::Bind() const
	{
		if (m_RendererID == 0)
			GX_ENGINE_ERROR("{0} could not be bound", m_FileName);
		else
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
				//GX_ENGINE_WARN("{0} : {1} uniform not present in the current bound shader", m_FileName, Name);
			}
			// Cache the location
			else
			{
				m_UniformLocations[Name] = location;
			}

			return location;
		}
	}

	void Shader::ParseFileName(const std::string& FilePath)
	{
		int length = FilePath.length();
		for (int i = length - 1; i >= 0; i--)
		{
			if (FilePath[i] != '/')
				m_FileName.push_back(FilePath[i]);
			else
				break;
		}

		std::reverse(m_FileName.begin(), m_FileName.end());
	}

	ShaderSource Shader::ParseShaderSource(const std::string& filePath)
	{
		GX_ENGINE_INFO("{0} : Parsing Shader source", m_FileName);
		Timer time(m_FileName +" : Parsing Shader Source");

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
		GX_ENGINE_INFO("{0} : Compiling Shaders", m_FileName);
		Timer time(m_FileName + " : Compiling Shaders");

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

			GX_ENGINE_ERROR("{0} : Failed to compile {1} shader",m_FileName, (type == GL_VERTEX_SHADER) ? "Vertex " : "Fragment ");
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