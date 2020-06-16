#include "GMPch.h"
#include "IntVector2.h"

#include "Vector3.h"
#include "MathUtility.h"

namespace GM
{
	// Define the zero vector 
	const IntVector2 IntVector2::ZeroVector(0, 0);
	
	// Define the unit vector 
	const IntVector2 IntVector2::UnitVector(1, 1);

	// Define the number of components
	const int IntVector2::Components = 2;

	IntVector2::IntVector2()
		:x(0), y(0)
	{}

	IntVector2::IntVector2(int32_t Value)
		: x(Value), y(Value)
	{}

	IntVector2::IntVector2(int32_t x, int32_t y)
		: x(x), y(y)
	{}

	IntVector2::IntVector2(const IntVector2& OtherVector)
		: x(OtherVector.x), y(OtherVector.y)
	{}

	IntVector2& IntVector2::operator=(const IntVector2& OtherVector)
	{
		x = OtherVector.x;
		y = OtherVector.y;
		return *this;
	}

	bool IntVector2::operator==(const IntVector2& OtherVector) const
	{
		return (x == OtherVector.x && y == OtherVector.y);
	}

	bool IntVector2::operator!=(const IntVector2& OtherVector) const
	{
		return !(*this == OtherVector);
	}

	const IntVector2 IntVector2::operator+(const IntVector2& OtherVector) const
	{
		const IntVector2 result(x + OtherVector.x, y + OtherVector.y);

		return result;
	}

	const IntVector2 IntVector2::operator-(const IntVector2& OtherVector) const
	{
		const IntVector2 result(x - OtherVector.x, y - OtherVector.y);
		return result;
	}

	const IntVector2 IntVector2::operator*(const IntVector2& OtherVector) const
	{
		IntVector2 result(x * OtherVector.x, y * OtherVector.y);

		return result;
	}

	const IntVector2 IntVector2::operator/(const IntVector2& OtherVector) const
	{
		int32_t xVal = OtherVector.x == 0 ? INT_MAX : x / OtherVector.x;
		int32_t yVal = OtherVector.y == 0 ? INT_MAX : y / OtherVector.y;

		IntVector2 result(xVal, yVal);
		return result;
	}

	const IntVector2 IntVector2::operator+(int32_t Value) const
	{
		IntVector2 result(x + Value, y + Value);

		return result;
	}

	const IntVector2 IntVector2::operator*(int32_t Value) const
	{
		IntVector2 result(x * Value, y * Value);

		return result;
	}

	const IntVector2 IntVector2::operator-(int32_t Value) const
	{
		IntVector2 result(x - Value, y - Value);

		return result;
	}

	const IntVector2 IntVector2::operator/(int32_t Value) const
	{
		int32_t xVal = (Value == 0) ? INT_MAX : x / Value;
		int32_t yVal = (Value == 0) ? INT_MAX : y / Value;

		IntVector2 result(xVal, yVal);
		return result;
	}

	/* Assignment operators */
	IntVector2& IntVector2::operator+=(const IntVector2& OtherVector)
	{
		*this = *this + OtherVector;

		return *this;
	}

	IntVector2& IntVector2::operator-=(const IntVector2& OtherVector)
	{
		*this = *this - OtherVector;

		return *this;
	}

	IntVector2& IntVector2::operator*=(const IntVector2& OtherVector)
	{
		*this = *this * OtherVector;

		return *this;
	}

	IntVector2& IntVector2::operator/=(const IntVector2& OtherVector)
	{
		*this = *this / OtherVector;

		return *this;
	}

	IntVector2& IntVector2::operator+=(int32_t Value)
	{
		*this = *this + Value;

		return *this;
	}

	IntVector2& IntVector2::operator-=(int32_t Value)
	{
		*this = *this - Value;

		return *this;
	}

	IntVector2& IntVector2::operator*=(int32_t Value)
	{
		*this = *this * Value;

		return *this;
	}

	IntVector2& IntVector2::operator/=(int32_t Value)
	{
		*this = *this / Value;

		return *this;
	}

	IntVector2 IntVector2::operator-() const
	{
		return (IntVector2::ZeroVector - *this);
	}

	/* Member functions */
	IntVector2 IntVector2::Reciprocal() const
	{
		IntVector2 result;
		result.x = x != 0 ? 1 / x : INT_MAX;
		result.y = y != 0 ? 1 / y : INT_MAX;

		return result;
	}

	IntVector2 IntVector2::Normal() const
	{
		IntVector2 result;
		int32_t magnitude = Magnitude();

		result.x = x / magnitude;
		result.y = y / magnitude;

		return result;
	}

	void IntVector2::Normalize()
	{
		int32_t magnitude = this->Magnitude();
		x /= magnitude;
		y /= magnitude;
	}

	int32_t IntVector2::Magnitude() const
	{
		return (int32_t)Utility::Sqrt((float)MagnitudeSquare());
	}

	int32_t IntVector2::MagnitudeSquare() const
	{
		return Utility::Square(x) + Utility::Square(y);
	}

	bool IntVector2::IsZero() const
	{
		return *this == IntVector2::ZeroVector;
	}

	/******** Static Methods *********/
	int32_t IntVector2::Distance(const IntVector2& V1, const IntVector2& V2)
	{
		return (int32_t)Utility::Sqrt((float)IntVector2::DistanceSquared(V1, V2));
	}

	int32_t IntVector2::DistanceSquared(const IntVector2& V1, const IntVector2& V2)
	{
		return (Utility::Square(V1.x - V2.x) + Utility::Square(V1.y - V2.y));
	}

	/* Non member functions */
	std::ostream& operator<<(std::ostream& OutStream, const IntVector2& Vector)
	{
		return OutStream << "X: " << Vector.x << " Y: " << Vector.y;
	}

	const IntVector2 operator+(int32_t Value, const IntVector2& Vector)
	{
		IntVector2 result(Vector.x + Value, Vector.y + Value);

		return result;
	}

	const IntVector2 operator*(int32_t Value, const IntVector2& Vector)
	{
		IntVector2 result(Vector.x * Value, Vector.y * Value);
		return result;
	}
}