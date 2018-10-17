#pragma once

namespace rtm
{
	namespace vector
	{
		class Vector2f
		{
		public:
			/* Contructors */
			explicit Vector2f();

			explicit Vector2f(float Value);

			explicit Vector2f(float x, float y);

			explicit Vector2f(const Vector2f& OtherVector);

			/* Relational Operators */
			Vector2f operator=(const Vector2f& OtherVector);

			Vector2f operator==(const Vector2f& OtherVector);

			Vector2f operator!=(const Vector2f& OtherVector);

			/* Arithmetic operators */
			const Vector2f operator+(const Vector2f& OtherVector);

			const Vector2f operator-(const Vector2f& OtherVector);

			const Vector2f operator*(const Vector2f& OtherVector);
  
		private:
			float m_x, m_y;
		};
	}
}