#include "GMPch.h"
#include "Plane.h"

#include "Vectors/Vector3.h"
#include "MathUtility.h"

namespace GM
{
	Plane::Plane(const Vector3& Normal, float InW)
		: x(Normal.x), y(Normal.y), z(Normal.z), w(InW)
	{
	}

	Plane::Plane(const Vector3& Normal, const Vector3& BasePoint)
		: x(Normal.x), y(Normal.y), z(Normal.z), w(Vector3::DotProduct(Normal, BasePoint))
	{
	}

	Plane::Plane(const Vector3& P1, const Vector3& P2, const Vector3& P3)
	{
		Vector3 Vec1 = P2 - P1;
		Vector3 Vec2 = P3 - P1;
		Vector3 Normal = Vector3::CrossProduct(Vec1, Vec2);
		
		x = Normal.x;
		y = Normal.y;
		z = Normal.z;
		w = Vector3::DotProduct(Normal, P1);
	}

	void Plane::Normalize()
	{
		const float Magnitude = x * x + y * y + z * z;
		if (Magnitude > 0)
		{
			const float Scale = 1.0f / Utility::Sqrt(Magnitude);
			x *= Scale; y *= Scale; z *= Scale; w *= Scale;
		}
	}

	float Plane::DistanceFrom(const Vector3& Point)
	{
		const float Magnitude = x * x + y * y + z * z;
		float Scale;
		if (Magnitude == 1.0f)
			Scale = 1.0f;
		else
			Scale = 1.0f / Utility::Sqrt(Magnitude);

		return Scale * (x * Point.x + y * Point.y + z * Point.z - w);
	}

	float Plane::DistanceFrom(const Plane& Other)
	{
		const float Magnitude = x * x + y * y + z * z;
		float Scale;
		if (Magnitude == 1.0f)
			Scale = 1.0f;
		else
			Scale = 1.0f / Utility::Sqrt(Magnitude);

		return Scale * (w - Other.w);
	}
}