#pragma once
#include <iostream>

namespace rtm
{
	namespace vector
	{
		struct Vector2
		{
		public:
			/* Vector's X - component */
			float x;

			/* Vector's Y - Component */
			float y;

			/* 2D Zero Vector constant(0, 0) */
			static const Vector2 ZeroVector;

			/* 2D Unit Vector constant(1, 1) */
			static const Vector2 UnitVector;

		public:
			/* Contructors */
			explicit Vector2();

			explicit Vector2(float Value);

			explicit Vector2(float x, float y);

			Vector2(const Vector2& OtherVector);

			/* Relational Operators */
			Vector2& operator=(const Vector2& OtherVector);

			bool operator==(const Vector2& OtherVector) const;

			bool operator!=(const Vector2& OtherVector) const;

			/* Arithmetic operators */
			const Vector2 operator+(const Vector2& OtherVector) const;

			const Vector2 operator-(const Vector2& OtherVector) const;

			const Vector2 operator*(const Vector2& OtherVector) const;

			const Vector2 operator/(const Vector2& OtherVector) const;

			const Vector2 operator+(float Value) const;

			const Vector2 operator-(float Value) const;

			const Vector2 operator*(float Value) const;

			const Vector2 operator/(float Value) const;

			/* Assignment operators */
			Vector2& operator+=(const Vector2& OtherVector);

			Vector2& operator-=(const Vector2& OtherVector);

			Vector2& operator*=(const Vector2& OtherVector);

			Vector2& operator/=(const Vector2& OtherVector);

			Vector2& operator+=(float Value);

			Vector2& operator-=(float Value);

			Vector2& operator*=(float Value);

			Vector2& operator/=(float Value);

		public:
			/*********** Static Member functions ***********/
			/* Dot product of the two vectors */
			static float DotProduct(const Vector2& V1, const Vector2& V2);

			/* Cross product of the two vectors */
			static float CrossProduct(const Vector2& V1, const Vector2& V2);

			/* Return the distance between two vectors */
			static float Distance(const Vector2& V1, const Vector2& V2);

			/* Return the square of the distance between two vectors */
			static float DistanceSquared(const Vector2& V1, const Vector2& V2);

		public:
			/* Returns the magnitude of the vector */
			float Magnitude() const;

			/* Returns the square of the magnitude of the vector */
			float MagnitudeSquare() const;

			/* Returns whether the vector is zero */
			bool IsZero() const;
		};

		/* Non Member functions */
		std::ostream& operator<<(std::ostream& OutStream, const Vector2& Vector);

		const Vector2 operator+(float Value, const Vector2& Vector);

		const Vector2 operator*(float Value, const Vector2& Vector);
	}
}