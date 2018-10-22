#pragma once

namespace rtm
{
	/* Maths utility for the maths library */
	struct MathUtil
	{
		/* Returns the square of the value */
		template<typename T>
		static float Square(const T Value)
		{
			return Value * Value;
		}

		/* Returns the sqrt of the value */
		template<typename T>
		static float Sqrt(const T Value)
		{
			if (Value > 0)
				return std::sqrt(Value);
			else
				return 0;
		}

		/* Returns min of the two values */
		template<typename T>
		static T Min(const T A, const T B)
		{
			return A > B ? B : A;
		}

		/* Returns max of the two values */
		template <typename T>
		static T Max(const T A, const T B)
		{
			return A > B ? A : B;
		}

		/************ Trignometric functions  **************/

	};
}