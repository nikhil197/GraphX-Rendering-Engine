#pragma once

#include "Matrices/Matrix4.h"
#include "MathUtility.h"

namespace GM
{
	class RotationTranslationMatrix
		: public Matrix4
	{
	public:
		/* Construct a combined rotation translaiton matrix based on the given values */
		RotationTranslationMatrix(const Vector3& Rot, const Vector3& Origin)
		{
			Make(*this, Rot, Origin);
		}

		/* Returns a combined matrix for given rotation and translation values */
		static Matrix4 Make(const Vector3& Rot, const Vector3& Origin)
		{
			return RotationTranslationMatrix(Rot, Origin);
		}

		/* Convert the given matrix into a combined rotation and translation matrix based on the given values */
		static void Make(Matrix4& Mat, const Vector3& Rot, const Vector3& Origin)
		{
			float cosA = (float)Utility::Cos(Rot.x);
			float cosB = (float)Utility::Cos(Rot.y);
			float cosC = (float)Utility::Cos(Rot.z);

			float sinA = (float)Utility::Sin(Rot.x);
			float sinB = (float)Utility::Sin(Rot.y);
			float sinC = (float)Utility::Sin(Rot.z);

			Mat(0, 0) = cosB * cosC;
			Mat(0, 1) = sinA * sinB * cosC - cosA * sinC;
			Mat(0, 2) = cosA * sinB * cosC + sinA * sinC;
			Mat(0, 3) = Origin.x;

			Mat(1, 0) = cosB * sinC;
			Mat(1, 1) = sinA * sinB * sinC + cosA * cosC;
			Mat(1, 2) = cosA * sinB * sinC - sinA * cosC;
			Mat(1, 3) = Origin.y;

			Mat(2, 0) = -sinB;
			Mat(2, 1) = sinA * cosB;
			Mat(2, 2) = cosA * cosB;
			Mat(2, 3) = Origin.z;

			// Offset the origin (Translation)
			Mat(3, 0) = 0.0f;
			Mat(3, 1) = 0.0f;
			Mat(3, 2) = 0.0f;
			Mat(3, 3) = 1.0f;
		}
	};
}