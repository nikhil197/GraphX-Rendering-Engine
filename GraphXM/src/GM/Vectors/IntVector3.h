#pragma once

namespace GM
{
	struct IntVector2;

	struct IntVector3
	{
	public:
		/* Constructors */
		explicit IntVector3();

		explicit IntVector3(int32_t Value);

		IntVector3(int32_t x, int32_t y, int32_t z);

		IntVector3(const IntVector2& Vec, int32_t z);

		IntVector3(int32_t x, const IntVector2& Vec);

		IntVector3(const IntVector3& OtherVector);

		/* Operators */
	public:
		IntVector3& operator=(const IntVector3& OtherVector);

		bool operator==(const IntVector3& OtherVector) const;

		bool operator!=(const IntVector3& OtherVector) const;

		/* Arithmetic Operators */
		const IntVector3 operator+(const IntVector3& OtherVector) const;

		const IntVector3 operator-(const IntVector3& OtherVector) const;

		const IntVector3 operator*(const IntVector3& OtherVector) const;

		const IntVector3 operator/(const IntVector3& OtherVector) const;

		const IntVector3 operator+(int32_t Value) const;

		const IntVector3 operator-(int32_t Value) const;

		const IntVector3 operator*(int32_t Value) const;

		const IntVector3 operator/(int32_t Value) const;

		/* Assignment Operators */
		IntVector3& operator+=(const IntVector3& OtherVector);

		IntVector3& operator-=(const IntVector3& OtherVector);

		IntVector3& operator*=(const IntVector3& OtherVector);

		IntVector3& operator/=(const IntVector3& OtherVector);

		IntVector3& operator+=(int32_t Value);

		IntVector3& operator-=(int32_t Value);

		IntVector3& operator*=(int32_t Value);

		IntVector3& operator/=(int32_t Value);

	public:
		/* Unary Negation operator */
		IntVector3 operator-() const;

		/* Returns the reciprocal of the vector (reciprocal of each component) */
		IntVector3 Reciprocal() const;

		/* Returns a normal vector of this vector */
		IntVector3 Normal() const;

		/* Converts this vector into a normalized vector */
		void Normalize();

		/* Returns the magnitude of the vector */
		int32_t Magnitude() const;

		/* Returns the square of magnitude of the vector */
		int32_t MagnitudeSquare() const;

		/* Returns whether vector is zero */
		bool IsZero() const;

	public:
		/*********** Static Member functions ***********/
		/* Return the distance between two vectors */
		static int32_t Distance(const IntVector3& V1, const IntVector3& V2);

		/* Return the square of the distance between two vectors */
		static int32_t DistanceSquared(const IntVector3& V1, const IntVector3& V2);

	public:
		/* X - Component of the vector */
		int32_t x;

		/* Y - Component of the vector */
		int32_t y;

		/* Z - Component of the vector */
		int32_t z;

		/* 3D Zero Vector constant (0, 0, 0)*/
		static const IntVector3 ZeroVector;

		/* 3D Unit Vector constant (1, 1, 1)*/
		static const IntVector3 UnitVector;

		/* Unit Vector Along x-axis */
		static const IntVector3 XAxis;

		/* Unit Vector Along y-axis */
		static const IntVector3 YAxis;

		/* Unit Vector Along z-axis */
		static const IntVector3 ZAxis;

		/* Number of components in the vector */
		static const int Components;
	};

	/* Non - Member functions */
	std::ostream& operator<<(std::ostream& out, const IntVector3& Vec);

	const IntVector3 operator+(int32_t Value, const IntVector3& Vec);

	const IntVector3 operator*(int32_t Value, const IntVector3& Vec);
}