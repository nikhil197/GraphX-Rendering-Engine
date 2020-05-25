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

		/* Returns floating point remainder of X / Y */
		static float Mod(float X, float Y)
		{
			float Quotient = X / Y;
			float Div = (float)((int)Quotient) * Y;

			// Rounding and imprecision could cause Div to exceed X and cause the result to be outside the expected range.
			if (fabsf(Div) > fabsf(X))
				Div = X;

			return X - Div;
		}

		/* Clamps an angle to the range [0, 360) */
		static float ClampAngle(float Angle)
		{
			Angle = Mod(Angle, 360.0f);
			if (Angle < 0.0f)
			{
				// shift to [0,360) range
				Angle += 360.0f;
			}

			return Angle;
		}

		/**
		* Clamps an arbitrary angle to be between the given angles.  Will clamp to nearest boundary.
		*
		* @param MinAngleDegrees	"from" angle that defines the beginning of the range of valid angles (sweeping clockwise)
		* @param MaxAngleDegrees	"to" angle that defines the end of the range of valid angles
		* @return Returns clamped angle in the range -180..180.
		*/
		static float ClampAngle(float Angle, const float Min, const float Max)
		{
			// Extent of the range
			const float Extent = ClampAngle(Max - Min) * 0.5f;

			// Center of the range [Min, Max]
			const float RangeCenter = ClampAngle(Min + Extent);

			// Offset of 'Angle' from the RangeCenter
			const float CenterOffset = NormalizeAngle(Angle - RangeCenter);

			// Greater than Max
			if (CenterOffset > Extent)
			{
				return NormalizeAngle(RangeCenter + Extent);
			}
			// Less than Min
			else if(CenterOffset < -Extent)
			{
				return NormalizeAngle(RangeCenter - Extent);
			}
			
			// In Range
			return NormalizeAngle(Angle);
		}

		/* Clamps an angle to the range [-180, 180) */
		static float NormalizeAngle(float Angle)
		{
			// returns Angle in the range [0,360)
			Angle = ClampAngle(Angle);

			if (Angle > 180.f)
			{
				// shift to (-180,180]
				Angle -= 360.f;
			}

			return Angle;
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