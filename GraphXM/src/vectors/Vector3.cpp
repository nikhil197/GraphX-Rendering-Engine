#include "Vector3.h"
#include "Vector2.h"

#include "../MathUtil.h"

namespace gm
{
	namespace vector
	{
		// Define the zero vector
		const Vector3 Vector3::ZeroVector(0, 0, 0);

		// Define the Unit Vector
		const Vector3 Vector3::UnitVector(1, 1, 1);

		Vector3::Vector3()
			:x(0), y(0), z(0)
		{}

		Vector3::Vector3(float Value)
			: x(Value), y(Value), z(Value)
		{}

		Vector3::Vector3(float x, float y, float z)
			: x(x), y(y), z(z)
		{}

		Vector3::Vector3(const Vector2& Vec, float z)
			: x(Vec.x), y(Vec.y), z(z)
		{}

		Vector3::Vector3(float x, const Vector2& Vec)
			: x(x), y(Vec.x), z(Vec.y)
		{}

		Vector3::Vector3(const Vector3& OtherVector)
			: x(OtherVector.x), y(OtherVector.y), z(OtherVector.z)
		{}

		/* Operators */
		Vector3& Vector3::operator=(const Vector3& OtherVector)
		{
			x = OtherVector.x;
			y = OtherVector.y;
			z = OtherVector.z;

			return *this;
		}

		bool Vector3::operator==(const Vector3& OtherVector) const
		{
			return (x == OtherVector.x && y == OtherVector.y && z == OtherVector.z);
		}

		bool Vector3::operator!=(const Vector3& OtherVector) const
		{
			return (x == OtherVector.x || y == OtherVector.y || z == OtherVector.z);
		}

		/* Arithmatic operators */
		const Vector3 Vector3::operator+(const Vector3& OtherVector) const
		{
			Vector3 result(x + OtherVector.x, y + OtherVector.y, z + OtherVector.z);
			return result;
		}

		const Vector3 Vector3::operator-(const Vector3& OtherVector) const
		{
			Vector3 result(x - OtherVector.x, y - OtherVector.y, z - OtherVector.z);
			return result;
		}

		const Vector3 Vector3::operator*(const Vector3& OtherVector) const
		{
			Vector3 result(x * OtherVector.x, y * OtherVector.y, z * OtherVector.z);
			return result;
		}

		const Vector3 Vector3::operator/(const Vector3& OtherVector) const
		{
			float xVal = OtherVector.x == 0 ? FLT_MAX : x / OtherVector.x;
			float yVal = OtherVector.y == 0 ? FLT_MAX : y / OtherVector.y;
			float zVal = OtherVector.z == 0 ? FLT_MAX : z / OtherVector.z;

			Vector3 result(xVal, yVal, zVal);
			return result;
		}

		const Vector3 Vector3::operator+(float Value) const
		{
			Vector3 result(x + Value, y + Value, z + Value);
			return result;
		}

		const Vector3 Vector3::operator-(float Value) const
		{
			Vector3 result(x - Value, y - Value, z - Value);
			return result;
		}

		const Vector3 Vector3::operator*(float Value) const
		{
			Vector3 result(x * Value, y * Value, z * Value);
			return result;
		}

		const Vector3 Vector3::operator/(float Value) const
		{
			float xVal = (Value == 0) ? FLT_MAX : x / Value;
			float yVal = (Value == 0) ? FLT_MAX : y / Value;
			float zVal = (Value == 0) ? FLT_MAX : z / Value;

			Vector3 result(xVal, yVal, zVal);
			return result;
		}

		/* Assignment operators */
		Vector3& Vector3::operator+=(const Vector3& OtherVector)
		{
			x += OtherVector.x;
			y += OtherVector.y;
			z += OtherVector.z;

			return *this;
		}

		Vector3& Vector3::operator-=(const Vector3& OtherVector)
		{
			x -= OtherVector.x;
			y -= OtherVector.y;
			z -= OtherVector.z;

			return *this;
		}

		Vector3& Vector3::operator*=(const Vector3& OtherVector)
		{
			x *= OtherVector.x;
			y *= OtherVector.y;
			z *= OtherVector.z;

			return *this;
		}

		Vector3& Vector3::operator/=(const Vector3& OtherVector)
		{
			x = (OtherVector.x == 0) ? FLT_MAX : x / OtherVector.x;
			y = (OtherVector.y == 0) ? FLT_MAX : y / OtherVector.y;
			z = (OtherVector.z == 0) ? FLT_MAX : z / OtherVector.z;

			return *this;
		}

		Vector3& Vector3::operator+=(float Value)
		{
			x += Value;
			y += Value;
			z += Value;

			return *this;
		}

		Vector3& Vector3::operator-=(float Value)
		{
			x += Value;
			y += Value;
			z += Value;

			return *this;
		}

		Vector3& Vector3::operator*=(float Value)
		{
			x *= Value;
			y *= Value;
			z *= Value;

			return *this;
		}

		Vector3& Vector3::operator/=(float Value)
		{
			x = (Value == 0) ? FLT_MAX : x / Value;
			y = (Value == 0) ? FLT_MAX : y / Value;
			z = (Value == 0) ? FLT_MAX : z / Value;

			return *this;
		}

		/* Member functions */
		float Vector3::Magnitude() const
		{
			return MathUtil::Sqrt(MagnitudeSquare());
		}

		float Vector3::MagnitudeSquare() const
		{
			return (MathUtil::Square(x) + MathUtil::Square(y) + MathUtil::Square(z));
		}

		bool Vector3::IsZero() const
		{
			return (*this == Vector3::ZeroVector);
		}

		/* Static Member functions */
		float Vector3::DotProduct(const Vector3& V1, const Vector3& V2)
		{
			return (V1.x * V2.x + V1.y * V2.y + V1.z * V2.z);
		}

		const Vector3 Vector3::CrossProduct(const Vector3& V1, const Vector3& V2)
		{
			Vector3 result((V1.y * V2.z - V1.z * V2.y), (V1.z * V2.x - V1.x * V2.z), (V1.x * V2.y - V1.y * V2.x));
			return result;
		}

		float Vector3::Distance(const Vector3& V1, const Vector3& V2)
		{
			return MathUtil::Sqrt(Vector3::DistanceSquared(V1, V2));
		}

		float Vector3::DistanceSquared(const Vector3& V1, const Vector3& V2)
		{
			return (MathUtil::Square(V1.x - V2.x) + MathUtil::Square(V1.y - V2.y) + MathUtil::Square(V1.z - V2.z));
		}
	}
}