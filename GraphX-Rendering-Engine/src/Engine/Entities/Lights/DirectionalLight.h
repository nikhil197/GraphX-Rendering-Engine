#pragma once

#include "Light.h"

namespace GraphX
{
	class DirectionalLight
		: public Light
	{
	public:
		/* Constructor */
		DirectionalLight(const GM::Vector4& Color, const GM::Vector3& LightDir, float Intensity = 1.0f);

		virtual void Update(float DeltaTime) override;

		virtual void Enable(class Shader& shader, const std::string& LightName = "u_DirectionalLight") const override;

		virtual void Disable() const override;

	public:
		/* Direction of the light */
		GM::Vector3 Direction;
	};
}