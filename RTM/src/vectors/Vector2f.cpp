#include "Vector2f.h"

namespace rtm
{
	namespace vector
	{
		Vector2f::Vector2f()
		{
			x = y = 0;
		}

		Vector2f::Vector2f(float Value)
		{
			x = y = Value;
		}

		Vector2f::Vector2f(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		Vector2f::Vector2f(const Vector2f& OtherVector)
		{
			x = OtherVector.x;
			y = OtherVector.y;
		}

		Vector2f& Vector2f::operator=(const Vector2f& OtherVector)
		{
			x = OtherVector.x;
			y = OtherVector.y;
			return *this;
		}

		bool Vector2f::operator==(const Vector2f& OtherVector)
		{
			return (x == OtherVector.x && y == OtherVector.y);
		}

		bool Vector2f::operator!=(const Vector2f& OtherVector)
		{
			return (x != OtherVector.x || y != OtherVector.y);
		}

		const Vector2f Vector2f::operator+(const Vector2f& OtherVector)
		{
			const Vector2f result(x + OtherVector.x, y + OtherVector.y);

			return result;
		}

		const Vector2f Vector2f::operator-(const Vector2f& OtherVector)
		{
			const Vector2f result(x - OtherVector.x, y - OtherVector.y);
			return result;
		}

		float Vector2f::operator*(const Vector2f& OtherVector)
		{
			return Dot(OtherVector);
		}

		const Vector2f& Vector2f::operator+=(const Vector2f& OtherVector)
		{
			x += OtherVector.x;
			y += OtherVector.y;

			return *this;
		}

		const Vector2f& Vector2f::operator-=(const Vector2f& OtherVector)
		{
			x -= OtherVector.x;
			y -= OtherVector.y;

			return *this;
		}

		const Vector2f Vector2f::operator+(int Value)
		{
			Vector2f result(x + Value, y + Value);

			return result;
		}

		const Vector2f Vector2f::operator*(int Value)
		{
			Vector2f result(x * Value, y * Value);

			return result;
		}

		const Vector2f Vector2f::operator-(int Value)
		{
			Vector2f result(x - Value, y - Value);

			return result;
		}

		const Vector2f Vector2f::operator/(int Value)
		{
			if (Value != 0)
			{
				Vector2f result(x / Value, y / Value);

				return result;
			}
		}

		float Vector2f::Dot(const Vector2f& OtherVector)
		{
			return ((x * OtherVector.x) + (y * OtherVector.y));
		}

		/* Non member functions */
		std::ostream& operator<<(std::ostream& OutStream, const Vector2f& Vector)
		{
			return OutStream << "X: " << Vector.x << " Y: " << Vector.y << std::endl;
		}

		const Vector2f operator+(int Value, const Vector2f& Vector)
		{
			Vector2f result(Vector.x + Value, Vector.y + Value);

			return result;
		}
	}
}