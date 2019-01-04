#pragma once
#include <iostream>

namespace gm
{
	struct Vector2;

	struct Vector4;

	struct Vector3
	{
	public:
		/* X - Component of the vector */
		float x;

		/* Y - Component of the vector */
		float y;

		/* Z - Component of the vector */
		float z;

		/* 3D Zero Vector constant (0, 0, 0)*/
		static const Vector3 ZeroVector;

		/* 3D Unit Vector constant (1, 1, 1)*/
		static const Vector3 UnitVector;

		/* Unit Vector Along x-axis */
		static const Vector3 XAxis;

		/* Unit Vector Along y-axis */
		static const Vector3 YAxis;

		/* Unit Vector Along z-axis */
		static const Vector3 ZAxis;

		/* Number of components in the vector */
		static const int Components;

	public:
		/* Constructors */
		explicit Vector3();

		explicit Vector3(float Value);

		Vector3(float x, float y, float z);

		Vector3(const Vector2& Vec, float z);

		Vector3(float x, const Vector2& Vec);

		Vector3(const Vector3& OtherVector);

		Vector3(const Vector4& Vec);

		/* Operators */
	public:
		Vector3& operator=(const Vector3& OtherVector);

		bool operator==(const Vector3& OtherVector) const;

		bool operator!=(const Vector3& OtherVector) const;

		/* Arithmetic Operators */
		const Vector3 operator+(const Vector3& OtherVector) const;

		const Vector3 operator-(const Vector3& OtherVector) const;

		const Vector3 operator*(const Vector3& OtherVector) const;

		const Vector3 operator/(const Vector3& OtherVector) const;

		const Vector3 operator+(float Value) const;

		const Vector3 operator-(float Value) const;

		const Vector3 operator*(float Value) const;

		const Vector3 operator/(float Value) const;

		/* Assignment Operators */
		Vector3& operator+=(const Vector3& OtherVector);

		Vector3& operator-=(const Vector3& OtherVector);

		Vector3& operator*=(const Vector3& OtherVector);

		Vector3& operator/=(const Vector3& OtherVector);

		Vector3& operator+=(float Value);

		Vector3& operator-=(float Value);

		Vector3& operator*=(float Value);

		Vector3& operator/=(float Value);

	public:
		/* Unary Negation operator */
		Vector3 operator-() const;

		/* Returns the reciprocal of the vector (reciprocal of each component) */
		Vector3 Reciprocal() const;

		/* Returns a normal vector of this vector */
		Vector3 Normal() const;

		/* Converts this vector into a normalized vector */
		void Normalize();

		/* Returns the magnitude of the vector */
		float Magnitude() const;

		/* Returns the square of magnitude of the vector */
		float MagnitudeSquare() const;

		/* Returns whether vector is zero */
		bool IsZero() const;

	public:
		/*********** Static Member functions ***********/
		/* Dot product of the two vectors */
		static float DotProduct(const Vector3& V1, const Vector3& V2);

		/* Cross product of the two vectors */
		static const Vector3 CrossProduct(const Vector3& V1, const Vector3& V2);

		/* Return the distance between two vectors */
		static float Distance(const Vector3& V1, const Vector3& V2);

		/* Return the square of the distance between two vectors */
		static float DistanceSquared(const Vector3& V1, const Vector3& V2);
	};

	/* Non - Member functions */
	std::ostream& operator<<(std::ostream& out, const Vector3& Vec);

	const Vector3 operator+(float Value, const Vector3& Vec);

	const Vector3 operator*(float Value, const Vector3& Vec);
}