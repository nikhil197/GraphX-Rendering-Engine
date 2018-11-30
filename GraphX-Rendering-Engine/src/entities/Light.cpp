#include "pch.h"
#include "Light.h"

namespace engine
{
	Light::Light(const gm::Vector3& Pos, const gm::Vector4& Color)
		: Position(Pos), Color(Color)
	{
	}
}