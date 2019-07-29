#include "Vector2.h"

#include "Vector3.h"
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

	Vector2 Vector2::operator-() const
	{
		return (Vector2::ZeroVector - *this);
	}

	/* Member functions */
	Vector2 Vector2::Reciprocal() const
	{
		Vector2 result;
		result.x = x != 0 ? 1 / x : FLT_MAX;
		result.y = y != 0 ? 1 / y : FLT_MAX;

		return result;
	}

	Vector2 Vector2::Normal() const
	{
		Vector2 result;
		float magnitude = Magnitude();

		result.x = x / magnitude;
		result.y = y / magnitude;

		return result;
	}

	void Vector2::Normalize()
	{
		float magnitude = this->Magnitude();
		x /= magnitude;
		y /= magnitude;
	}

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

	const Vector3 Vector2::CrossProduct(const Vector2& V1, const Vector2& V2)
	{
		return gm::Vector3::CrossProduct(Vector3(V1, 0), Vector3(V2, 0));
		//return (V1.x * V2.y - V1.y * V2.x);
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
		return OutStream << "X: " << Vector.x << " Y: " << Vector.y;
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