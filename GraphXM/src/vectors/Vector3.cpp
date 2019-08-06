#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

#include "../MathUtil.h"

namespace GraphXMaths
{
	// Define the zero vector
	const Vector3 Vector3::ZeroVector(0, 0, 0);

	// Define the Unit Vector
	const Vector3 Vector3::UnitVector(1, 1, 1);

	// Define the X - Axis
	const Vector3 Vector3::XAxis(1, 0, 0);

	// Define the Y - Axis
	const Vector3 Vector3::YAxis(0, 1, 0);
	
	// Define the Z - Axis
	const Vector3 Vector3::ZAxis(0, 0, 1);

	// Define the number of components in the vector
	const int Vector3::Components = 3;
	
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

	Vector3::Vector3(const Vector4& Vec)
		:x(Vec.x / Vec.w), y(Vec.y / Vec.w), z(Vec.z / Vec.w)
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
		return !(*this == OtherVector);
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
		*this = *this + OtherVector;

		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& OtherVector)
	{
		*this = *this - OtherVector;

		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& OtherVector)
	{
		*this = *this * OtherVector;

		return *this;
	}

	Vector3& Vector3::operator/=(const Vector3& OtherVector)
	{
		*this = *this / OtherVector;

		return *this;
	}

	Vector3& Vector3::operator+=(float Value)
	{
		*this = *this + Value;

		return *this;
	}

	Vector3& Vector3::operator-=(float Value)
	{
		*this = *this - Value;

		return *this;
	}

	Vector3& Vector3::operator*=(float Value)
	{
		*this = *this - Value;

		return *this;
	}

	Vector3& Vector3::operator/=(float Value)
	{
		*this = *this / Value;

		return *this;
	}

	Vector3 Vector3::operator-() const
	{
		return Vector3::ZeroVector - *this;
	}

	/* Member functions */
	Vector3 Vector3::Reciprocal() const
	{
		Vector3 result;
		result.x = x != 0 ? 1 / x : FLT_MAX;
		result.y = y != 0 ? 1 / y : FLT_MAX;
		result.z = z != 0 ? 1 / z : FLT_MAX;

		return result;
	}
	
	Vector3 Vector3::Normal() const
	{
		Vector3 result;
		float magnitude = Magnitude();

		result.x = x / magnitude;
		result.y = y / magnitude;
		result.z = z / magnitude;

		return result;
	}

	void Vector3::Normalize()
	{
		float magnitude = this->Magnitude();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	float Vector3::Magnitude() const
	{
		return Sqrt(MagnitudeSquare());
	}

	float Vector3::MagnitudeSquare() const
	{
		return (Square(x) + Square(y) + Square(z));
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
		return Sqrt(Vector3::DistanceSquared(V1, V2));
	}

	float Vector3::DistanceSquared(const Vector3& V1, const Vector3& V2)
	{
		return (Square(V1.x - V2.x) + Square(V1.y - V2.y) + Square(V1.z - V2.z));
	}

	/* Non - member functions */
	std::ostream& operator<<(std::ostream& Out, const Vector3& Vec)
	{
		return Out << "X: " << Vec.x << " Y: " << Vec.y << " Z: " << Vec.z;
	}

	const Vector3 operator+(float Value, const Vector3& Vec)
	{
		Vector3 result(Value + Vec.x, Value + Vec.y, Value + Vec.z);
		return result;
	}

	const Vector3 operator*(float Value, const Vector3& Vec)
	{
		Vector3 result(Value * Vec.x, Value * Vec.y, Value * Vec.z);
		return result;
	}
}