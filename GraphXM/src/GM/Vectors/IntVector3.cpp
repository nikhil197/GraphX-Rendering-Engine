#include "GMPch.h"
#include "IntVector3.h"

#include "IntVector2.h"
#include "MathUtility.h"

namespace GM
{
	// Define the zero vector
	const IntVector3 IntVector3::ZeroVector(0, 0, 0);

	// Define the Unit Vector
	const IntVector3 IntVector3::UnitVector(1, 1, 1);

	// Define the X - Axis
	const IntVector3 IntVector3::XAxis(1, 0, 0);

	// Define the Y - Axis
	const IntVector3 IntVector3::YAxis(0, 1, 0);

	// Define the Z - Axis
	const IntVector3 IntVector3::ZAxis(0, 0, 1);

	// Define the number of components in the vector
	const int IntVector3::Components = 3;

	IntVector3::IntVector3()
		:x(0), y(0), z(0)
	{}

	IntVector3::IntVector3(int32_t Value)
		: x(Value), y(Value), z(Value)
	{}

	IntVector3::IntVector3(int32_t x, int32_t y, int32_t z)
		: x(x), y(y), z(z)
	{}

	IntVector3::IntVector3(const IntVector2& Vec, int32_t z)
		: x(Vec.x), y(Vec.y), z(z)
	{}

	IntVector3::IntVector3(int32_t x, const IntVector2& Vec)
		: x(x), y(Vec.x), z(Vec.y)
	{}

	IntVector3::IntVector3(const IntVector3& OtherVector)
		: x(OtherVector.x), y(OtherVector.y), z(OtherVector.z)
	{}

	/* Operators */
	IntVector3& IntVector3::operator=(const IntVector3& OtherVector)
	{
		x = OtherVector.x;
		y = OtherVector.y;
		z = OtherVector.z;

		return *this;
	}

	bool IntVector3::operator==(const IntVector3& OtherVector) const
	{
		return (x == OtherVector.x && y == OtherVector.y && z == OtherVector.z);
	}

	bool IntVector3::operator!=(const IntVector3& OtherVector) const
	{
		return !(*this == OtherVector);
	}

	/* Arithmetic operators */
	const IntVector3 IntVector3::operator+(const IntVector3& OtherVector) const
	{
		IntVector3 result(x + OtherVector.x, y + OtherVector.y, z + OtherVector.z);
		return result;
	}

	const IntVector3 IntVector3::operator-(const IntVector3& OtherVector) const
	{
		IntVector3 result(x - OtherVector.x, y - OtherVector.y, z - OtherVector.z);
		return result;
	}

	const IntVector3 IntVector3::operator*(const IntVector3& OtherVector) const
	{
		IntVector3 result(x * OtherVector.x, y * OtherVector.y, z * OtherVector.z);
		return result;
	}

	const IntVector3 IntVector3::operator/(const IntVector3& OtherVector) const
	{
		int32_t xVal = OtherVector.x == 0 ? INT_MAX : x / OtherVector.x;
		int32_t yVal = OtherVector.y == 0 ? INT_MAX : y / OtherVector.y;
		int32_t zVal = OtherVector.z == 0 ? INT_MAX : z / OtherVector.z;

		IntVector3 result(xVal, yVal, zVal);
		return result;
	}

	const IntVector3 IntVector3::operator+(int32_t Value) const
	{
		IntVector3 result(x + Value, y + Value, z + Value);
		return result;
	}

	const IntVector3 IntVector3::operator-(int32_t Value) const
	{
		IntVector3 result(x - Value, y - Value, z - Value);
		return result;
	}

	const IntVector3 IntVector3::operator*(int32_t Value) const
	{
		IntVector3 result(x * Value, y * Value, z * Value);
		return result;
	}

	const IntVector3 IntVector3::operator/(int32_t Value) const
	{
		int32_t xVal = (Value == 0) ? INT_MAX : x / Value;
		int32_t yVal = (Value == 0) ? INT_MAX : y / Value;
		int32_t zVal = (Value == 0) ? INT_MAX : z / Value;

		IntVector3 result(xVal, yVal, zVal);
		return result;
	}

	/* Assignment operators */
	IntVector3& IntVector3::operator+=(const IntVector3& OtherVector)
	{
		*this = *this + OtherVector;

		return *this;
	}

	IntVector3& IntVector3::operator-=(const IntVector3& OtherVector)
	{
		*this = *this - OtherVector;

		return *this;
	}

	IntVector3& IntVector3::operator*=(const IntVector3& OtherVector)
	{
		*this = *this * OtherVector;

		return *this;
	}

	IntVector3& IntVector3::operator/=(const IntVector3& OtherVector)
	{
		*this = *this / OtherVector;

		return *this;
	}

	IntVector3& IntVector3::operator+=(int32_t Value)
	{
		*this = *this + Value;

		return *this;
	}

	IntVector3& IntVector3::operator-=(int32_t Value)
	{
		*this = *this - Value;

		return *this;
	}

	IntVector3& IntVector3::operator*=(int32_t Value)
	{
		*this = *this - Value;

		return *this;
	}

	IntVector3& IntVector3::operator/=(int32_t Value)
	{
		*this = *this / Value;

		return *this;
	}

	IntVector3 IntVector3::operator-() const
	{
		return IntVector3::ZeroVector - *this;
	}

	/* Member functions */
	IntVector3 IntVector3::Reciprocal() const
	{
		IntVector3 result;
		result.x = x != 0 ? 1 / x : INT_MAX;
		result.y = y != 0 ? 1 / y : INT_MAX;
		result.z = z != 0 ? 1 / z : INT_MAX;

		return result;
	}

	IntVector3 IntVector3::Normal() const
	{
		IntVector3 result;
		int32_t magnitude = Magnitude();

		result.x = x / magnitude;
		result.y = y / magnitude;
		result.z = z / magnitude;

		return result;
	}

	void IntVector3::Normalize()
	{
		int32_t magnitude = this->Magnitude();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	int32_t IntVector3::Magnitude() const
	{
		return Utility::Sqrt(MagnitudeSquare());
	}

	int32_t IntVector3::MagnitudeSquare() const
	{
		return (Utility::Square(x) + Utility::Square(y) + Utility::Square(z));
	}

	bool IntVector3::IsZero() const
	{
		return (*this == IntVector3::ZeroVector);
	}

	/* Static Member functions */
	int32_t IntVector3::Distance(const IntVector3& V1, const IntVector3& V2)
	{
		return (int32_t)(Utility::Sqrt((float)IntVector3::DistanceSquared(V1, V2)));
	}

	int32_t IntVector3::DistanceSquared(const IntVector3& V1, const IntVector3& V2)
	{
		return (Utility::Square(V1.x - V2.x) + Utility::Square(V1.y - V2.y) + Utility::Square(V1.z - V2.z));
	}

	/* Non - member functions */
	std::ostream& operator<<(std::ostream& Out, const IntVector3& Vec)
	{
		return Out << "X: " << Vec.x << " Y: " << Vec.y << " Z: " << Vec.z;
	}

	const IntVector3 operator+(int32_t Value, const IntVector3& Vec)
	{
		IntVector3 result(Value + Vec.x, Value + Vec.y, Value + Vec.z);
		return result;
	}

	const IntVector3 operator*(int32_t Value, const IntVector3& Vec)
	{
		IntVector3 result(Value * Vec.x, Value * Vec.y, Value * Vec.z);
		return result;
	}
}