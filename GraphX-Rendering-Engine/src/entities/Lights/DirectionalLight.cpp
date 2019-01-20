#include "pch.h"
#include "DirectionalLight.h"

#include "Shaders/Shader.h"

namespace engine
{
	DirectionalLight::DirectionalLight(const gm::Vector3& Pos, const gm::Vector4& Color, const gm::Vector3& LightDir, float Intensity)
		: Light(Pos, Color, Intensity), Direction(LightDir)
	{
	}

	void DirectionalLight::Enable(Shader& shader, const std::string& LightName) const
	{
		Light::Enable(shader, LightName);

		shader.SetUniform3f((LightName + ".Direction").c_str(), Direction);
	}

	void DirectionalLight::Disable() const
	{
		Light::Disable();
	}
}