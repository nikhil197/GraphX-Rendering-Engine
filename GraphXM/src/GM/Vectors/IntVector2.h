#pragma once

namespace GM
{
	struct IntVector2
	{
	public:
		/* Constructors */
		explicit IntVector2();

		explicit IntVector2(int32_t Value);

		IntVector2(int32_t x, int32_t y);

		IntVector2(const IntVector2& OtherVector);

		/* Relational Operators */
		IntVector2& operator=(const IntVector2& OtherVector);

		bool operator==(const IntVector2& OtherVector) const;

		bool operator!=(const IntVector2& OtherVector) const;

		/* Arithmetic operators */
		const IntVector2 operator+(const IntVector2& OtherVector) const;

		const IntVector2 operator-(const IntVector2& OtherVector) const;

		const IntVector2 operator*(const IntVector2& OtherVector) const;

		const IntVector2 operator/(const IntVector2& OtherVector) const;

		const IntVector2 operator+(int32_t Value) const;

		const IntVector2 operator-(int32_t Value) const;

		const IntVector2 operator*(int32_t Value) const;

		const IntVector2 operator/(int32_t Value) const;

		/* Assignment operators */
		IntVector2& operator+=(const IntVector2& OtherVector);

		IntVector2& operator-=(const IntVector2& OtherVector);

		IntVector2& operator*=(const IntVector2& OtherVector);

		IntVector2& operator/=(const IntVector2& OtherVector);

		IntVector2& operator+=(int32_t Value);

		IntVector2& operator-=(int32_t Value);

		IntVector2& operator*=(int32_t Value);

		IntVector2& operator/=(int32_t Value);

	public:
		/*********** Static Member functions ***********/
		/* Return the distance between two vectors */
		static int32_t Distance(const IntVector2& V1, const IntVector2& V2);

		/* Return the square of the distance between two vectors */
		static int32_t DistanceSquared(const IntVector2& V1, const IntVector2& V2);

	public:
		/* Unary Negation operator */
		IntVector2 operator-() const;

		/* Returns the reciprocal of the vector (reciprocal of each component)*/
		IntVector2 Reciprocal() const;

		/* Returns a normal vector(unit vector) of this vector */
		IntVector2 Normal() const;

		/* Converts this vector into normalized vector */
		void Normalize();

		/* Returns the magnitude of the vector */
		int32_t Magnitude() const;

		/* Returns the square of the magnitude of the vector */
		int32_t MagnitudeSquare() const;

		/* Returns whether the vector is zero */
		bool IsZero() const;

	public:
		/* Vector's X - component */
		int32_t x;

		/* Vector's Y - Component */
		int32_t y;

		/* 2D Zero Vector constant(0, 0) */
		static const IntVector2 ZeroVector;

		/* Number of components in the vector */
		static const int Components;
	};

	/* Non Member functions */
	std::ostream& operator<<(std::ostream& OutStream, const IntVector2& Vector);

	const IntVector2 operator+(int32_t Value, const IntVector2& Vector);

	const IntVector2 operator*(int32_t Value, const IntVector2& Vector);
}