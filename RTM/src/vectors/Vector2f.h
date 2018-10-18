#pragma once
#include <iostream>

namespace rtm
{
	namespace vector
	{
		class Vector2f
		{
		public:
			float x, y;

			/* Contructors */
			explicit Vector2f();

			explicit Vector2f(float Value);

			explicit Vector2f(float x, float y);

			Vector2f(const Vector2f& OtherVector);

			/* Relational Operators */
			Vector2f& operator=(const Vector2f& OtherVector);

			bool operator==(const Vector2f& OtherVector);

			bool operator!=(const Vector2f& OtherVector);

			/* Arithmetic operators */
			const Vector2f operator+(const Vector2f& OtherVector);

			const Vector2f operator-(const Vector2f& OtherVector);

			float operator*(const Vector2f& OtherVector);
  
			const Vector2f& operator+=(const Vector2f& OtherVector);

			const Vector2f& operator-=(const Vector2f& OtherVector);

			const Vector2f operator+(int Value);

			const Vector2f operator-(int Value);

			const Vector2f operator*(int Value);

			const Vector2f operator/(int Value);

			/* Dot product of the two vectors */
			float Dot(const Vector2f& OtherVector);
		};

		/* Non Member functions */
		std::ostream& operator<<(std::ostream& OutStream, const Vector2f& Vector);
		
		const Vector2f operator+(int Value, const Vector2f& Vector);
	}
}