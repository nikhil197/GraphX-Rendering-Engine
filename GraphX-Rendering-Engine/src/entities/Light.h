#pragma once

#include "GraphX_Vectors.h"

namespace engine
{
	class Light
	{
	public:
		/* The position of the light */
		gm::Vector3 Position;

		/* Color of the light */
		gm::Vector4 Color;

		/* Constructor */
		Light(const gm::Vector3& Pos, const gm::Vector4& Color);
	};
}