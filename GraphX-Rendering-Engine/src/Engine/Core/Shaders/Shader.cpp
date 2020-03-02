#include "pch.h"
#include "Shader.h"
#include "GL/glew.h"

#include "Utilities/EngineUtil.h"
#include "Timer/Timer.h"

namespace GraphX
{
	Shader::Shader(const std::string& filePath, const std::string& name)
		:m_Name(name), m_RendererID(0)
	{
		GX_PROFILE_FUNCTION()

		if (name == "")
		{
			m_Name = EngineUtil::ExtractFileName(filePath);
		}

		ShaderSource source = ParseShaderSource(filePath);
		
		if (source.FragmentShaderSource.length() > 0 && source.VertexShaderSource.length() > 0)
			m_RendererID = CreateShader(source.VertexShaderSource, source.FragmentShaderSource);
		else
			GX_ENGINE_ERROR("Error while creating the shader {0}, Source not found", m_Name);
	}

	Shader::Shader(const std::string& name, const std::string& vertexShaderSrc, const std::string& fragShaderSrc)
		: m_Name(name), m_RendererID(0)
	{
		GX_PROFILE_FUNCTION()

		m_RendererID = CreateShader(vertexShaderSrc, fragShaderSrc);
	}

	Shader::~Shader()
	{
		GX_PROFILE_FUNCTION()

		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		GX_PROFILE_FUNCTION()

		if (m_RendererID == 0)
			GX_ENGINE_ERROR("'{0}' shader could not be bound", m_Name);
		else
			glUseProgram(m_RendererID);
	}

	void Shader::UnBind() const
	{
		GX_PROFILE_FUNCTION()

		glUseProgram(0);
	}

	void Shader::SetUniform1i(const char* Name, int Val)
	{
		glUniform1i(GetLocation(Name), Val);
	}

	void Shader::SetUniform1iv(const char* Name, uint32_t count, const int* vals)
	{
		glUniform1iv(GetLocation(Name), count, vals);
	}

	void Shader::SetUniform2i(const char* Name, int v1, int v2)
	{
		glUniform2i(GetLocation(Name), v1, v2);
	}

	void Shader::SetUniform1f(const char* Name, float Val)
	{
		glUniform1f(GetLocation(Name), Val);
	}

	void Shader::SetUniform3f(const char* Name, float r, float g, float b)
	{
		glUniform3f(GetLocation(Name), r, g, b);
	}

	void Shader::SetUniform2f(const char* Name, float r, float g)
	{
		glUniform2f(GetLocation(Name), r, g);
	}

	void Shader::SetUniform2f(const char* Name, const GM::Vector2& Vec)
	{
		glUniform2f(GetLocation(Name), Vec.x, Vec.y);
	}

	void Shader::SetUniform3f(const char* Name, const GM::Vector3& Vec)
	{
		glUniform3f(GetLocation(Name), Vec.x, Vec.y, Vec.z);
	}

	void Shader::SetUniform4f(const char* Name, float r, float g, float b, float a)
	{
		glUniform4f(GetLocation(Name), r, g, b, a);
	}

	void Shader::SetUniform4f(const char* Name, const GM::Vector4& Vec)
	{
		glUniform4f(GetLocation(Name), Vec.x, Vec.y, Vec.z, Vec.w);
	}

	void Shader::SetUniform4f(const char* Name, const GM::Vector2& Vec1, const GM::Vector2& Vec2)
	{
		glUniform4f(GetLocation(Name), Vec1.x, Vec1.y, Vec2.x, Vec2.y);
	}

	void Shader::SetUniformMat3f(const char* Name, const GM::Matrix3& Mat)
	{
		glUniformMatrix3fv(GetLocation(Name), 1, GL_TRUE, &Mat[0][0]);
	}

	void Shader::SetUniformMat4f(const char* Name, const GM::Matrix4& Mat)
	{
		glUniformMatrix4fv(GetLocation(Name), 1, GL_TRUE, &Mat[0][0]);
	}

	int Shader::GetLocation(const char* Name)
	{
		GX_PROFILE_FUNCTION()

		// First check if the uniform location is cached and return it
		if (m_UniformLocations.find(Name) != m_UniformLocations.end())
			return m_UniformLocations[Name];
		else
		{
			// Get the location of the uniform
			int location = glGetUniformLocation(m_RendererID, Name);
				
			//If the name is invalid
			if (location == -1)
			{
				//GX_ENGINE_WARN("{0} : {1} uniform not present in the current bound shader", m_Name, Name);
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
		GX_ENGINE_INFO("'{0}' shader : Parsing Shader source", m_Name);
		GX_PROFILE_FUNCTION()

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
		GX_ENGINE_INFO("'{0}' shader : Compiling {1} Shader", m_Name, type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
		GX_PROFILE_FUNCTION()

		int shaderID = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shaderID, 1, &src, nullptr);
		glCompileShader(shaderID);

		int result;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

			char* infoLog = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(shaderID, length, &length, infoLog);

			GX_ENGINE_ERROR("'{0}' shader : Failed to compile {1} shader",m_Name, (type == GL_VERTEX_SHADER) ? "Vertex " : "Fragment ");
			GX_ENGINE_ERROR(infoLog);

			glDeleteShader(shaderID);
			return 0;
		}

		return shaderID;
	}

	unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		GX_PROFILE_FUNCTION()

		int programID = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		glAttachShader(programID, vs);
		glAttachShader(programID, fs);
		glLinkProgram(programID);
		glValidateProgram(programID);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return programID;
	}
}