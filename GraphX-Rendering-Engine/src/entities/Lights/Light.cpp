#include "pch.h"
#include "Light.h"

#include "Shaders/Shader.h"

namespace engine
{
	Light::Light(const gm::Vector3& Pos, const gm::Vector4& Color, gm::Matrix4 ProjMat, float Intensity)
		: Position(Pos), Color(Color), Intensity(Intensity), bShowDetails(true), m_LightViewProjMat(gm::Matrix4()), m_LightProjMatrix(ProjMat)
	{
	}

	void Light::Update(float DeltaTime)
	{
		m_LightViewProjMat = m_LightProjMatrix * gm::View::LookAt(Position, gm::Vector3(0.0f), gm::Vector3(0.0, 1.0, 0.0));
	}

	void Light::Enable(Shader& shader, const std::string& LightName) const
	{
		shader.SetUniform3f((LightName + ".Position").c_str(), Position);
		shader.SetUniform4f((LightName + ".Color").c_str(), Color);
		shader.SetUniform1f((LightName + ".Intensity").c_str(), Intensity);

		if(GX_ENABLE_SHADOWS)
			SetLightSpaceMatrix(shader, LightName);
	}

	void Light::Disable() const
	{
	}

	void Light::SetLightSpaceMatrix(Shader& DepthShader, const std::string& LightName) const
	{
		DepthShader.SetUniformMat4f((LightName + ".LightSpaceMatrix").c_str(), m_LightViewProjMat);
	}

	Light::~Light()
	{
	}
}