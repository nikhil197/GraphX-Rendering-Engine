#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"

#include "../MathUtil.h"

namespace gm
{
	namespace vector
	{
		// Define the Zero vector
		const Vector4 Vector4::ZeroVector(0, 0, 0, 0);

		// Define the Unit vector
		const Vector4 Vector4::UnitVector(1, 1, 1, 1);

		// Define the number of components in the vector
		const int Vector4::Components = 4;

		Vector4::Vector4()
			: w(0), x(0), y(0), z(0)
		{}

		Vector4::Vector4(float Value)
			: w(Value), x(Value), y(Value), z(Value)
		{}

		Vector4::Vector4(float w, float x, float y, float z)
			: w(w), x(x), y(y), z(z)
		{}

		Vector4::Vector4(const Vector2& Vec, float y, float z)
			: w(Vec.x), x(Vec.y), y(y), z(z)
		{}

		Vector4::Vector4(float w, const Vector2& Vec, float z)
			: w(w), x(Vec.x), y(Vec.y), z(z)
		{}

		Vector4::Vector4(float w, float x, const Vector2& Vec)
			: w(w), x(x), y(Vec.x), z(Vec.y)
		{}

		Vector4::Vector4(const Vector3& Vec, float z)
			: w(Vec.x), x(Vec.y), y(Vec.z), z(z)
		{}

		Vector4::Vector4(float w, const Vector3& Vec)
			: w(w), x(Vec.x), y(Vec.y), z(Vec.z)
		{}

		// Operators
		Vector4& Vector4::operator=(const Vector4& OtherVector)
		{
			w = OtherVector.w;
			x = OtherVector.x;
			y = OtherVector.y;
			z = OtherVector.z;

			return *this;
		}

		bool Vector4::operator==(const Vector4& OtherVector) const
		{
			return (w == OtherVector.w && x == OtherVector.x && y == OtherVector.y && z == OtherVector.z);
		}

		bool Vector4::operator!=(const Vector4& OtherVector) const
		{
			return !(*this == OtherVector);
		}

		// Arithmetic Operators
		const Vector4 Vector4::operator+(const Vector4& OtherVector) const
		{
			Vector4 result(w + OtherVector.w, x + OtherVector.x, y + OtherVector.y, z + OtherVector.z);
			return result;
		}

		const Vector4 Vector4::operator-(const Vector4& OtherVector) const
		{
			Vector4 result(w - OtherVector.w, x - OtherVector.x, y - OtherVector.y, z - OtherVector.z);
			return result;
		}

		const Vector4 Vector4::operator*(const Vector4& OtherVector) const
		{
			Vector4 result(w * OtherVector.w, x * OtherVector.x, y * OtherVector.y, z * OtherVector.z);
			return result;
		}

		const Vector4 Vector4::operator/(const Vector4& OtherVector) const
		{
			float wVal = OtherVector.w == 0 ? FLT_MAX : w / OtherVector.w;
			float xVal = OtherVector.x == 0 ? FLT_MAX : x / OtherVector.x;
			float yVal = OtherVector.y == 0 ? FLT_MAX : y / OtherVector.y;
			float zVal = OtherVector.z == 0 ? FLT_MAX : z / OtherVector.z;

			Vector4 result(wVal, xVal, yVal, zVal);
			return result;
		}

		const Vector4 Vector4::operator+(float Value) const
		{
			Vector4 result(w + Value, x + Value, y + Value, z + Value);
			return result;
		}

		const Vector4 Vector4::operator-(float Value) const
		{
			Vector4 result(w - Value, x - Value, y - Value, z - Value);
			return result;
		}

		const Vector4 Vector4::operator*(float Value) const
		{
			Vector4 result(w * Value, x * Value, y * Value, z * Value);
			return result;
		}

		const Vector4 Vector4::operator/(float Value) const
		{
			float wVal = (Value == 0) ? FLT_MAX : w / Value;
			float xVal = (Value == 0) ? FLT_MAX : x / Value;
			float yVal = (Value == 0) ? FLT_MAX : y / Value;
			float zVal = (Value == 0) ? FLT_MAX : z / Value;

			Vector4 result(wVal, xVal, yVal, zVal);
			return result;
		}

		// Assignment Operators
		Vector4& Vector4::operator+=(const Vector4& OtherVector)
		{
			*this = *this + OtherVector;

			return *this;
		}

		Vector4& Vector4::operator-=(const Vector4& OtherVector)
		{
			*this = *this - OtherVector;

			return *this;
		}

		Vector4& Vector4::operator*=(const Vector4& OtherVector)
		{
			*this = *this * OtherVector;

			return *this;
		}

		Vector4& Vector4::operator/=(const Vector4& OtherVector)
		{
			*this = *this / OtherVector;

			return *this;
		}

		Vector4& Vector4::operator+=(float Value)
		{
			*this = *this + Value;

			return *this;
		}

		Vector4& Vector4::operator-=(float Value)
		{
			*this = *this - Value;

			return *this;
		}

		Vector4& Vector4::operator*=(float Value)
		{
			*this = *this * Value;

			return *this;
		}

		Vector4& Vector4::operator/=(float Value)
		{
			*this = *this / Value;

			return *this;
		}

		// Member Functions
		float Vector4::Magnitude() const
		{
			return MathUtil::Sqrt(MagnitudeSquare());
		}

		float Vector4::MagnitudeSquare() const
		{
			return (MathUtil::Square(w) + MathUtil::Square(x) + MathUtil::Square(y) + MathUtil::Square(z));
		}

		bool Vector4::IsZero() const
		{
			return (*this == Vector4::ZeroVector);
		}

		float Vector4::DotProduct(const Vector4& V1, const Vector4& V2)
		{
			return (V1.w * V2.w + V1.x * V2.x + V1.y * V2.y + V1.z * V2.z);
		}

		// To be Completed
		const Vector4 Vector4::CrossProduct(const Vector4& V1, const Vector4 & V2)
		{
			return Vector4();
		}

		float Vector4::Distance(const Vector4& V1, const Vector4 & V2)
		{
			return MathUtil::Sqrt(Vector4::DistanceSquared(V1, V2));
		}

		float Vector4::DistanceSquared(const Vector4& V1, const Vector4 & V2)
		{
			return (MathUtil::Square(V1.w - V2.w) + MathUtil::Square(V1.x - V2.x) + MathUtil::Square(V1.y - V2.y) + MathUtil::Square(V1.z - V2.z));
		}
	}
}

