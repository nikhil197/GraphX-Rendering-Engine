#include "pch.h"
#include "Light.h"

#include "Shaders/Shader.h"

namespace engine
{
	Light::Light(const gm::Vector3& Pos, const gm::Vector4& Color, float Intensity)
		: Position(Pos), Color(Color), Intensity(Intensity)
	{
	}

	void Light::Enable(Shader& shader) const
	{
		shader.SetUniform3f("u_LightPos", Position);
		shader.SetUniform3f("u_LightColor", Color);
		shader.SetUniform1f("u_LightIntensity", Intensity);
	}

	void Light::Disable() const
	{

	}

	Light::~Light()
	{

	}
}