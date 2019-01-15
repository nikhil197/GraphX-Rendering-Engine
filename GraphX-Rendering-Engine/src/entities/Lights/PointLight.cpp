#include "pch.h"

#include "PointLight.h"
#include "Shaders/Shader.h"

namespace engine
{
	PointLight::PointLight(const gm::Vector3& Pos, const gm::Vector4& Col, float Intensity, const gm::Vector3& AttenuationFactors)
		: Light(Pos, Col, Intensity), m_AttenuationFactors(AttenuationFactors)
	{
	}

	void PointLight::Enable(Shader& shader) const
	{
		Light::Enable(shader);
		
		// Set the attenuation factors for the light
		shader.SetUniform3f("u_AttenuationFactors", m_AttenuationFactors);
	}

	void PointLight::Disable() const
	{
		Light::Disable();
	}

	PointLight::~PointLight()
	{
	}
}