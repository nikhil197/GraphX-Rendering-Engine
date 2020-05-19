#pragma once

#include "GMPch.h"

/** 
 * A Utility for the mathematics library containing the common mathematics functions
 **/
namespace GM
{
	// Define the value of PI (static to make it private to this translation unit i.e. the translation unit in which this header file will be included)
	static constexpr float PI = 3.14159265f;

	// Multiplier used to convert angles from degree to radians
	static constexpr float DEG_TO_RADS = PI / 180;

	// Multiplier used to convert angles from degree to radians
	static constexpr float RADS_TO_DEG = 1 / DEG_TO_RADS;

	struct Utility
	{
		/* Returns the square of the value */
		template<typename T>
		static T Square(const T Value)
		{
			return Value * Value;
		}

		/* Returns the sqrt of the value */
		static float Sqrt(const float Value)
		{
			if (Value > 0)
				return std::sqrtf(Value);
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

		template<typename T>
		static void ClampAngle(T& Val)
		{
			if (Val > 360)
				Val = Val - 360;
			else if (Val < 0)
				Val = Val + 360;
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
		static void SinAndCos(float& OutSin, float& OutCos, float AngleInDegrees)
		{
			OutSin = Sin(AngleInDegrees);
			OutCos = Cos(AngleInDegrees);
		}

		// Cosine Function
		static float Cos(float angleInDegrees)
		{
			return cosf(angleInDegrees * DEG_TO_RADS);
		}

		// Secant Function
		static float Sec(float angleInDegrees)
		{
			float val = Cos(angleInDegrees);

			if (val == 0)
				return std::numeric_limits<float>::max();
			else
				return 1 / val;
		}

		// Sine Function
		static float Sin(float angleInDegrees)
		{
			return sinf(angleInDegrees * DEG_TO_RADS);
		}

		// Cosecant Function
		static float Csc(float angleInDegrees)
		{
			float val = Sin(angleInDegrees);

			if (val == 0)
				return std::numeric_limits<float>::max();
			else
				return 1 / val;
		}

		// Tangent Function
		static float Tan(float angleInDegrees)
		{
			return tanf(angleInDegrees * DEG_TO_RADS);
		}

		// Cotangent Function
		static float Cot(float angleInDegrees)
		{
			float val = Tan(angleInDegrees);

			if (val == 0)
				return std::numeric_limits<float>::max();
			else
				return 1 / val;
		}

		/************ Inverse Trignometric functions  **************/

		// Inverse Cosine Function
		static float ACos(float Val)
		{
			// Make sure that the Val lies within the valid Domain for the inverse operation
			Clamp(Val, -1.0f, 1.0f);
			return acosf(Val) * RADS_TO_DEG;
		}

		// Inverse Sine Function
		static float ASin(float Val)
		{
			// Make sure that the Val lies within the valid Domain for the inverse operation
			Clamp(Val, -1.0f, 1.0f);
			return asinf(Val) * RADS_TO_DEG;
		}

		// Inverse Tangent Function
		static float ATan(float Val)
		{
			return atanf(Val) * RADS_TO_DEG;
		}

		// Linear Interpolation between A and B based on BlendFactor(B/W 0 and 1)
		template<typename T>
		static T Lerp(const T& A, const T& B, float BlendFactor)
		{
			return (1 - BlendFactor) * A + BlendFactor * B;
		}
	};
}