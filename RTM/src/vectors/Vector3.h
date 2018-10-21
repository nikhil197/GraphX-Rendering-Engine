#pragma once

namespace rtm
{
	namespace vector
	{
		struct Vector2;

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

		public:
			/* Constructors */
			Vector3();

			Vector3(float Value);

			Vector3(float x, float y, float z);

			Vector3(const Vector2& Vec, float z);

			Vector3(float x, const Vector2& Vec);

			Vector3(const Vector3& OtherVector);

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
	}
}