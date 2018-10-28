#include "Vector2.h"

#include "../MathUtil.h"

namespace gm
{
	// Define the zero vector 
	const Vector2 Vector2::ZeroVector(0, 0);

	// Define the unit vector
	const Vector2 Vector2::UnitVector(1, 1);

	// Define the number of components
	const int Vector2::Components = 2;

	Vector2::Vector2()
		:x(0), y(0)
	{}

	Vector2::Vector2(float Value)
		: x(Value), y(Value)
	{}

	Vector2::Vector2(float x, float y)
		: x(x), y(y)
	{}

	Vector2::Vector2(const Vector2& OtherVector)
		: x(OtherVector.x), y(OtherVector.y)
	{}

	Vector2& Vector2::operator=(const Vector2& OtherVector)
	{
		x = OtherVector.x;
		y = OtherVector.y;
		return *this;
	}

	bool Vector2::operator==(const Vector2& OtherVector) const
	{
		return (x == OtherVector.x && y == OtherVector.y);
	}

	bool Vector2::operator!=(const Vector2& OtherVector) const
	{
		return !(*this == OtherVector);
	}

	const Vector2 Vector2::operator+(const Vector2& OtherVector) const
	{
		const Vector2 result(x + OtherVector.x, y + OtherVector.y);

		return result;
	}

	const Vector2 Vector2::operator-(const Vector2& OtherVector) const
	{
		const Vector2 result(x - OtherVector.x, y - OtherVector.y);
		return result;
	}

	const Vector2 Vector2::operator*(const Vector2& OtherVector) const
	{
		Vector2 result(x * OtherVector.x, y * OtherVector.y);

		return result;
	}

	const Vector2 Vector2::operator/(const Vector2& OtherVector) const
	{
		float xVal = OtherVector.x == 0 ? FLT_MAX : x / OtherVector.x;
		float yVal = OtherVector.y == 0 ? FLT_MAX : y / OtherVector.y;

		Vector2 result(xVal, yVal);
		return result;
	}

	const Vector2 Vector2::operator+(float Value) const
	{
		Vector2 result(x + Value, y + Value);

		return result;
	}

	const Vector2 Vector2::operator*(float Value) const
	{
		Vector2 result(x * Value, y * Value);

		return result;
	}

	const Vector2 Vector2::operator-(float Value) const
	{
		Vector2 result(x - Value, y - Value);

		return result;
	}

	const Vector2 Vector2::operator/(float Value) const
	{
		float xVal = (Value == 0) ? FLT_MAX : x / Value;
		float yVal = (Value == 0) ? FLT_MAX : y / Value;

		Vector2 result(xVal, yVal);
		return result;
	}

	/* Assignment operators */
	Vector2& Vector2::operator+=(const Vector2& OtherVector)
	{
		*this = *this + OtherVector;

		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& OtherVector)
	{
		*this = *this - OtherVector;

		return *this;
	}

	Vector2& Vector2::operator*=(const Vector2& OtherVector)
	{
		*this = *this * OtherVector;

		return *this;
	}

	Vector2& Vector2::operator/=(const Vector2& OtherVector)
	{
		*this = *this / OtherVector;

		return *this;
	}

	Vector2& Vector2::operator+=(float Value)
	{
		*this = *this + Value;

		return *this;
	}

	Vector2& Vector2::operator-=(float Value)
	{
		*this = *this - Value;

		return *this;
	}

	Vector2& Vector2::operator*=(float Value)
	{
		*this = *this * Value;

		return *this;
	}

	Vector2& Vector2::operator/=(float Value)
	{
		*this = *this / Value;

		return *this;
	}

	/* Member functions */
	float Vector2::Magnitude() const
	{
		return MathUtil::Sqrt(MagnitudeSquare());
	}

	float Vector2::MagnitudeSquare() const
	{
		return MathUtil::Square(x) + MathUtil::Square(y);
	}

	bool Vector2::IsZero() const
	{
		return *this == Vector2::ZeroVector;
	}

	/******** Static Methods *********/
	float Vector2::DotProduct(const Vector2& V1, const Vector2& V2)
	{
		return (V1.x * V2.x + V1.y * V2.y);
	}

	float Vector2::CrossProduct(const Vector2& V1, const Vector2& V2)
	{
		return (V1.x * V2.y - V1.y * V2.x);
	}

	float Vector2::Distance(const Vector2& V1, const Vector2& V2)
	{
		return MathUtil::Sqrt(Vector2::DistanceSquared(V1, V2));
	}

	float Vector2::DistanceSquared(const Vector2& V1, const Vector2& V2)
	{
		return (MathUtil::Square(V1.x - V2.x) + MathUtil::Square(V1.y - V2.y));
	}

	/* Non member functions */
	std::ostream& operator<<(std::ostream& OutStream, const Vector2& Vector)
	{
		return OutStream << "X: " << Vector.x << " Y: " << Vector.y << std::endl;
	}

	const Vector2 operator+(float Value, const Vector2& Vector)
	{
		Vector2 result(Vector.x + Value, Vector.y + Value);

		return result;
	}

	const Vector2 operator*(float Value, const Vector2& Vector)
	{
		Vector2 result(Vector.x * Value, Vector.y * Value);
		return result;
	}
}