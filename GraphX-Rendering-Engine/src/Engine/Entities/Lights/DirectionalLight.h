#pragma once

#include "Light.h"

namespace GraphX
{
	class DirectionalLight
		: public Light
	{
	public:
		/* Direction of the light */
		GraphXMaths::Vector3 Direction;

		/* Constructor */
		DirectionalLight(const GraphXMaths::Vector4& Color, const GraphXMaths::Vector3& LightDir, float Intensity = 1.0f);

		virtual void Update(float DeltaTime) override;

		virtual void Enable(class Shader& shader, const std::string& LightName = "u_DirectionalLight") const override;

		virtual void Disable() const override;
	};
}