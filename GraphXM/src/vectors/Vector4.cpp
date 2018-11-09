#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"

#include "../MathUtil.h"

namespace gm
{
	// Define the Zero vector
	const Vector4 Vector4::ZeroVector(0, 0, 0, 0);

	// Define the Unit vector
	const Vector4 Vector4::UnitVector(1, 1, 1, 1);

	// Define the number of components in the vector
	const int Vector4::Components = 4;

	Vector4::Vector4()
		: x(0), y(0), z(0), w(0)
	{}

	Vector4::Vector4(float Value)
		: x(Value), y(Value), z(Value), w(Value)
	{}

	Vector4::Vector4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{}

	Vector4::Vector4(const Vector2& Vec, float z, float w)
		: x(Vec.x), y(Vec.y), z(z), w(w)
	{}

	Vector4::Vector4(float x, const Vector2& Vec, float w)
		: x(x), y(Vec.x), z(Vec.y), w(w)
	{}

	Vector4::Vector4(float x, float y, const Vector2& Vec)
		: x(x), y(y), z(Vec.x), w(Vec.y)
	{}

	Vector4::Vector4(const Vector3& Vec, float w)
		: x(Vec.x), y(Vec.y), z(Vec.z), w(w)
	{}

	Vector4::Vector4(float x, const Vector3& Vec)
		: x(x), y(Vec.x), z(Vec.y), w(Vec.z)
	{}

	// Operators
	Vector4& Vector4::operator=(const Vector4& OtherVector)
	{
		x = OtherVector.x;
		y = OtherVector.y;
		z = OtherVector.z;
		w = OtherVector.w;

		return *this;
	}

	bool Vector4::operator==(const Vector4& OtherVector) const
	{
		return (x == OtherVector.x && y == OtherVector.y && z == OtherVector.z && w == OtherVector.w);
	}

	bool Vector4::operator!=(const Vector4& OtherVector) const
	{
		return !(*this == OtherVector);
	}

	// Arithmetic Operators
	const Vector4 Vector4::operator+(const Vector4& OtherVector) const
	{
		Vector4 result(x + OtherVector.x, y + OtherVector.y, z + OtherVector.z, w + OtherVector.w);
		return result;
	}

	const Vector4 Vector4::operator-(const Vector4& OtherVector) const
	{
		Vector4 result(x - OtherVector.x, y - OtherVector.y, z - OtherVector.z, w - OtherVector.w);
		return result;
	}

	const Vector4 Vector4::operator*(const Vector4& OtherVector) const
	{
		Vector4 result(x * OtherVector.x, y * OtherVector.y, z * OtherVector.z, w * OtherVector.w);
		return result;
	}

	const Vector4 Vector4::operator/(const Vector4& OtherVector) const
	{
		float xVal = OtherVector.x == 0 ? FLT_MAX : x / OtherVector.x;
		float yVal = OtherVector.y == 0 ? FLT_MAX : y / OtherVector.y;
		float zVal = OtherVector.z == 0 ? FLT_MAX : z / OtherVector.z;
		float wVal = OtherVector.w == 0 ? FLT_MAX : w / OtherVector.w;

		Vector4 result(xVal, yVal, zVal, wVal);
		return result;
	}

	const Vector4 Vector4::operator+(float Value) const
	{
		Vector4 result(x + Value, y + Value, z + Value, w + Value);
		return result;
	}

	const Vector4 Vector4::operator-(float Value) const
	{
		Vector4 result(x - Value, y - Value, z - Value, w - Value);
		return result;
	}

	const Vector4 Vector4::operator*(float Value) const
	{
		Vector4 result(x * Value, y * Value, z * Value, w * Value);
		return result;
	}

	const Vector4 Vector4::operator/(float Value) const
	{
		float xVal = (Value == 0) ? FLT_MAX : x / Value;
		float yVal = (Value == 0) ? FLT_MAX : y / Value;
		float zVal = (Value == 0) ? FLT_MAX : z / Value;
		float wVal = (Value == 0) ? FLT_MAX : w / Value;

		Vector4 result(xVal, yVal, zVal, wVal);
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

	Vector4 Vector4::operator-() const
	{
		return Vector4::ZeroVector - *this;
	}

	// Member Functions
	Vector4 Vector4::Reciprocal() const
	{
		Vector4 result;
		result.x = x != 0 ? 1 / x : FLT_MAX;
		result.y = y != 0 ? 1 / y : FLT_MAX;
		result.z = z != 0 ? 1 / z : FLT_MAX;
		result.w = w != 0 ? 1 / w : FLT_MAX;

		return result;
	}

	float Vector4::Magnitude() const
	{
		return MathUtil::Sqrt(MagnitudeSquare());
	}

	float Vector4::MagnitudeSquare() const
	{
		return (MathUtil::Square(x) + MathUtil::Square(y) + MathUtil::Square(z) + MathUtil::Square(w));
	}

	bool Vector4::IsZero() const
	{
		return (*this == Vector4::ZeroVector);
	}

	Vector4 Vector4::Normal() const
	{
		Vector4 result;
		float magnitude = Magnitude();

		result.x = x / magnitude;
		result.y = y / magnitude;
		result.z = z / magnitude;
		result.w = w / magnitude;

		return result;
	}

	float Vector4::DotProduct(const Vector4& V1, const Vector4& V2)
	{
		return (V1.x * V2.x + V1.y * V2.y + V1.z * V2.z + V1.w * V2.w);
	}

	// To be Completed
	const Vector4 Vector4::CrossProduct(const Vector4& V1, const Vector4& V2)
	{
		return Vector4();
	}

	float Vector4::Distance(const Vector4& V1, const Vector4& V2)
	{
		return MathUtil::Sqrt(Vector4::DistanceSquared(V1, V2));
	}

	float Vector4::DistanceSquared(const Vector4& V1, const Vector4& V2)
	{
		return (MathUtil::Square(V1.x - V2.x) + MathUtil::Square(V1.y - V2.y) + MathUtil::Square(V1.z - V2.z) + MathUtil::Square(V1.w - V2.w));
	}

	/* Non - Member functions */
	std::ostream& operator<<(std::ostream& Out, const Vector4& Vec)
	{
		return Out << "X: " << Vec.x << " Y: " << Vec.y << " Z: " << Vec.z << " W: " << Vec.w << std::endl;
	}

	const Vector4 operator+(int Value, const Vector4& Vec)
	{
		Vector4 result(Value + Vec.x, Value + Vec.y, Value + Vec.z, Value + Vec.w);
		return result;
	}

	const Vector4 operator*(int Value, const Vector4& Vec)
	{
		Vector4 result(Value * Vec.x, Value * Vec.y, Value * Vec.z, Value * Vec.w);
		return result;
	}
}

