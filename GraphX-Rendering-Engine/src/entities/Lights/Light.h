#pragma once

namespace engine
{
	class Light
	{
	public:
		/* The position of the light */
		gm::Vector3 Position;

		/* Color of the light */
		gm::Vector4 Color;

		/* Intensity of the light (total energy emitted by the light) */
		float Intensity;

		/* Constructor */
		Light(const gm::Vector3& Pos, const gm::Vector4& Color, float Intensity = 1.0f);

		/* Enable the light for rendering */
		virtual void Enable(class Shader& shader) const;

		/* Disable the light after rendering is done */
		virtual void Disable() const;

		/* Destructor */
		virtual ~Light();
	};
}