#include "pch.h"
#include "DirectionalLight.h"

#include "Shaders/Shader.h"
#include "Entities/Camera.h"

namespace engine
{
	DirectionalLight::DirectionalLight(const gm::Vector3& Pos, const gm::Vector4& Color, const gm::Vector3& LightDir, float Intensity)
		: Light(Pos, Color, gm::Projection::Ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1000.0f), Intensity), Direction(LightDir)
	{
	}

	void DirectionalLight::Update(float DeltaTime)
	{
		Light::Update(DeltaTime);
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