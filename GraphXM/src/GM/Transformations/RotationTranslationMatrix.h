#pragma once

#include "Matrices/Matrix4.h"
#include "Misc/Rotator.h"
#include "MathUtility.h"

namespace GM
{
	class RotationTranslationMatrix
		: public Matrix4
	{
	public:
		/* Construct a combined rotation translaiton matrix based on the given values */
		RotationTranslationMatrix(const Rotator& Rot, const Vector3& Origin)
		{
			Make(*this, Rot, Origin);
		}

		/* Construct a combined rotation (about given axis) translaiton matrix based on the given values */
		RotationTranslationMatrix(float Angle, const Vector3& RotAxis, const Vector3& Origin)
		{
			Make(*this, Angle, RotAxis, Origin);
		}

		/* Returns a combined matrix for given rotation and translation values */
		static Matrix4 Make(const Rotator& Rot, const Vector3& Origin)
		{
			return RotationTranslationMatrix(Rot, Origin);
		}

		/* Returns a combined matrix for given rotation (about given axis) and translation values */
		static Matrix4 Make(float Angle, const Vector3& RotAxis, const Vector3& Origin)
		{
			return RotationTranslationMatrix(Angle, RotAxis, Origin);
		}

		/* Convert the given matrix into a combined rotation and translation matrix based on the given values */
		static void Make(Matrix4& Mat, const Rotator& Rot, const Vector3& Origin)
		{
			float CP = Utility::Cos(Rot.Pitch);
			float CY = Utility::Cos(Rot.Yaw);
			float CR = Utility::Cos(Rot.Roll);

			float SP = Utility::Sin(Rot.Pitch);
			float SY = Utility::Sin(Rot.Yaw);
			float SR = Utility::Sin(Rot.Roll);

			Mat(0, 0) = CY * CR;
			Mat(0, 1) = SP * SY * CR - CP * SR;
			Mat(0, 2) = CP * SY * CR + SP * SR;
			Mat(0, 3) = Origin.x;

			Mat(1, 0) = CY * SR;
			Mat(1, 1) = SP * SY * SR + CP * CR;
			Mat(1, 2) = CP * SY * SR - SP * CR;
			Mat(1, 3) = Origin.y;

			Mat(2, 0) = -SY;
			Mat(2, 1) = SP * CY;
			Mat(2, 2) = CP * CY;
			Mat(2, 3) = Origin.z;

			Mat(3, 0) = 0.0f;
			Mat(3, 1) = 0.0f;
			Mat(3, 2) = 0.0f;
			Mat(3, 3) = 1.0f;
		}

		/* Convert the given matrix into a combined rotation (about given axis) and translation matrix based on the given values */
		static void Make(Matrix4& Mat, float Angle, const Vector3& RotAxis, const Vector3& Origin)
		{
			// Get the normalized Axis
			Vector3 nAxis = RotAxis.Normal();

			float cos = Utility::Cos(Angle);
			float sin = Utility::Sin(Angle);
			float oneMinusCos = 1 - cos;

			float xy = nAxis.x * nAxis.y;
			float yz = nAxis.y * nAxis.z;
			float zx = nAxis.z * nAxis.x;

			Mat(0, 0) = cos + (nAxis.x * nAxis.x) * oneMinusCos;
			Mat(0, 1) = xy * oneMinusCos - nAxis.z * sin;
			Mat(0, 2) = zx * oneMinusCos + nAxis.y * sin;
			Mat(0, 3) = Origin.x;

			Mat(1, 0) = xy * oneMinusCos + nAxis.z * sin;
			Mat(1, 1) = cos + (nAxis.y * nAxis.y) * oneMinusCos;
			Mat(1, 2) = yz * oneMinusCos - nAxis.x * sin;
			Mat(1, 3) = Origin.y;

			Mat(2, 0) = zx * oneMinusCos - nAxis.y * sin;
			Mat(2, 1) = yz * oneMinusCos + nAxis.x * sin;
			Mat(2, 2) = cos + (nAxis.z * nAxis.z) * oneMinusCos;
			Mat(2, 3) = Origin.z;

			Mat(3, 0) = 0.0f;
			Mat(3, 1) = 0.0f;
			Mat(3, 2) = 0.0f;
			Mat(3, 3) = 1.0f;
		}
	};
}