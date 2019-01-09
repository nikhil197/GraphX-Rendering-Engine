#pragma once

// Define the value of PI (static to make it private to this translation unit i.e. the translation unit in which this header file will be included)
static const double PI = 3.14159265;

namespace gm
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

		/* Clamps the value between the specified range */
		template<typename T>
		static void Clamp(T& Val, const T& Min, const T& Max)
		{
			if (Val < Min)
				Val = Min;
			else if (Val > Max)
				Val = Max;
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

		/** Returns the absolute value */
		template <typename T>
		static T Abs(const T A)
		{
			return A > 0 ? A : -A;
		}

		/************ Trignometric functions  **************/

		// Cosine Function
		template <typename T>
		static double Cos(T angleInDegrees)
		{
			return cos(angleInDegrees * PI / 180);
		}

		// Secant Function
		template <typename T>
		static double Sec(T angleInDegrees)
		{
			double val = Cos(angleInDegrees);

			if (val == 0)
				return DBL_MAX;
			else
				return 1 / val;
		}

		// Sine Function
		template <typename T>
		static double Sin(T angleInDegrees)
		{
			return sin(angleInDegrees * PI / 180);
		}

		// Cosecant Function
		template <typename T>
		static double Csc(T angleInDegrees)
		{
			double val = Sin(angleInDegrees);

			if (val == 0)
				return DBL_MAX;
			else
				return 1 / val;
		}

		// Tangent Function
		template <typename T>
		static double Tan(T angleInDegrees)
		{
			return tan(angleInDegrees * PI / 180);
		}

		// Cotangent Function
		template <typename T>
		static double Cot(T angleInDegrees)
		{
			double val = Tan(angleInDegrees);

			if (val == 0)
				return DBL_MAX;
			else
				return 1 / val;
		}

		/************ Inverse Trignometric functions  **************/

		// Inverse Cosine Function
		template <typename T>
		static double CosInverse(T Val)
		{
			return acos(Val) * 180 / PI;
		}

		// Inverse Sine Function
		template <typename T>
		static double SinInverse(T Val)
		{
			return asin(Val) * 180 / PI;
		}

		// Inverse Tangent Function
		template <typename T>
		static double TanInverse(T Val)
		{
			return atan(Val) * 180 / PI;
		}
	};
}