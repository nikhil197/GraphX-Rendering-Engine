#include "pch.h"

#include "PointLight.h"
#include "Shaders/Shader.h"
#include "Entities/Camera.h"

namespace engine
{
	PointLight::PointLight(const gm::Vector3& Pos, const gm::Vector4& Col, float Intensity, const gm::Vector3& AttenuationFactors)
		: Light(Pos, Col, gm::Projection::Ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 1000.0f), Intensity), m_AttenuationFactors(AttenuationFactors)
	{
	}

	void PointLight::Update(float DeltaTime)
	{
		Light::Update(DeltaTime);
	}

	void PointLight::Enable(Shader& shader, const std::string& LightName) const
	{
		Light::Enable(shader, LightName);
		
		// Set the attenuation factors for the light
		shader.SetUniform3f((LightName + ".AttenuationFactors").c_str(), m_AttenuationFactors);
	}

	void PointLight::Disable() const
	{
		Light::Disable();
	}

	PointLight::~PointLight()
	{
	}
}