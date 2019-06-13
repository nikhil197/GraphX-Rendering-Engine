#include "pch.h"
#include "Light.h"

#include "Shaders/Shader.h"

namespace engine
{
	Light::Light(const gm::Vector4& Color, gm::Matrix4 ProjMat, float Intensity)
		: Color(Color), Intensity(Intensity), bShowDetails(true), m_LightShadowInfo(new LightShadowInfo(ProjMat, gm::Matrix4()))
	{
	}

	void Light::Update(float DeltaTime)
	{
	}

	void Light::Enable(Shader& shader, const std::string& LightName) const
	{
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
		DepthShader.SetUniformMat4f((LightName + ".LightSpaceMatrix").c_str(), m_LightShadowInfo->LightViewProjMat);
	}

	Light::~Light()
	{
	}
}