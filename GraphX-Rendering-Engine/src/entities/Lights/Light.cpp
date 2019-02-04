#include "pch.h"
#include "Light.h"

#include "Shaders/Shader.h"

namespace engine
{
	Light::Light(const gm::Vector3& Pos, const gm::Vector4& Color, float Intensity)
		: Position(Pos), Color(Color), Intensity(Intensity), bShowDetails(true)
	{
	}

	void Light::Enable(Shader& shader, const std::string& LightName) const
	{
		shader.SetUniform3f((LightName + ".Position").c_str(), Position);
		shader.SetUniform3f((LightName + ".Color").c_str(), Color);
		shader.SetUniform1f((LightName + ".Intensity").c_str(), Intensity);
	}

	void Light::Disable() const
	{
	}

	Light::~Light()
	{
	}
}