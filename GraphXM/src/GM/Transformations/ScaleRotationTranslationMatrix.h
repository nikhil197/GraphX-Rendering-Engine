#pragma once

#include "Matrices/Matrix4.h"
#include "MathUtility.h"
#include "Misc/Rotator.h"

namespace GM
{
	class ScaleRotationTranslationMatrix
		: public Matrix4
	{
	public:
		/* Construct a combined scale, rotation and translation matrix based on the given values */
		ScaleRotationTranslationMatrix(const Vector3& Scale, const Rotator& Rot, const Vector3& Origin)
		{
			Make(*this, Scale, Rot, Origin);
		}

		/* Construct a combined scale, rotation (about given axis) and translation matrix based on the given values */
		ScaleRotationTranslationMatrix(const Vector3& Scale, float Angle, const Vector3& RotAxis, const Vector3& Origin)
		{
			Make(*this, Scale, Angle, RotAxis, Origin);
		}

		/* Returns a combined matrix for given scale, rotation and translation values */
		static Matrix4 Make(const Vector3& Scale, const Rotator& Rot, const Vector3& Origin)
		{
			return ScaleRotationTranslationMatrix(Scale, Rot, Origin);
		}

		/* Returns a combined matrix for given scale, rotation (about a given axis) and translation values */
		static Matrix4 Make(const Vector3& Scale, float Angle, const Vector3& RotAxis, const Vector3& Origin)
		{
			return ScaleRotationTranslationMatrix(Scale, Angle, RotAxis, Origin);
		}

		/* Convert a given matrix into a combined scale, rotation and translation matrix based on the given values */
		static void Make(Matrix4& Mat, const Vector3& Scale, const Rotator& Rot, const Vector3& Origin)
		{
			float CP = Utility::Cos(Rot.Pitch);
			float CY = Utility::Cos(Rot.Yaw);
			float CR = Utility::Cos(Rot.Roll);

			float SP = Utility::Sin(Rot.Pitch);
			float SY = Utility::Sin(Rot.Yaw);
			float SR = Utility::Sin(Rot.Roll);

			Mat(0, 0) = (CP * CY) * Scale.x;
			Mat(0, 1) = (SR * SP * CY - CR * SY) * Scale.y;
			Mat(0, 2) = (CR * SP * CY + SR * SY) * Scale.z;
			Mat(0, 3) = Origin.x;

			Mat(1, 0) = (CP * SY) * Scale.x;
			Mat(1, 1) = (SR * SP * SY + CR * CY) * Scale.y;
			Mat(1, 2) = (CR * SP * SY - SR * CY) * Scale.z;
			Mat(1, 3) = Origin.y;

			Mat(2, 0) = (-SP) * Scale.x;
			Mat(2, 1) = (SR * CP) * Scale.y;
			Mat(2, 2) = (CR * CP) * Scale.z;
			Mat(2, 3) = Origin.z;

			Mat(3, 0) = 0.0f;
			Mat(3, 1) = 0.0f;
			Mat(3, 2) = 0.0f;
			Mat(3, 3) = 1.0f;
		}

		/* Convert a given matrix into a combined scale, rotation (about a given axis) and translation matrix based on the given values */
		static void Make(Matrix4& Mat, const Vector3& Scale, float Angle, const Vector3& RotAxis, const Vector3& Origin)
		{
			// Get the normalized Axis
			Vector3 nAxis = RotAxis.Normal();

			float cos = Utility::Cos(Angle);
			float sin = Utility::Sin(Angle);
			float oneMinusCos = 1 - cos;

			float xy = nAxis.x * nAxis.y;
			float yz = nAxis.y * nAxis.z;
			float zx = nAxis.z * nAxis.x;

			Mat(0, 0) = (cos + (nAxis.x * nAxis.x) * oneMinusCos) * Scale.x;
			Mat(0, 1) = (xy * oneMinusCos - nAxis.z * sin) * Scale.y;
			Mat(0, 2) = (zx * oneMinusCos + nAxis.y * sin) * Scale.z;
			Mat(0, 3) = Origin.x;
						
			Mat(1, 0) = (xy * oneMinusCos + nAxis.z * sin) * Scale.x;
			Mat(1, 1) = (cos + (nAxis.y * nAxis.y) * oneMinusCos) * Scale.y;
			Mat(1, 2) = (yz * oneMinusCos - nAxis.x * sin) * Scale.z;
			Mat(1, 3) = Origin.y;
						
			Mat(2, 0) = (zx * oneMinusCos - nAxis.y * sin) * Scale.x;
			Mat(2, 1) = (yz * oneMinusCos + nAxis.x * sin) * Scale.y;
			Mat(2, 2) = (cos + (nAxis.z * nAxis.z) * oneMinusCos) * Scale.z;
			Mat(2, 3) = Origin.z;

			Mat(3, 0) = 0.0f;
			Mat(3, 1) = 0.0f;
			Mat(3, 2) = 0.0f;
			Mat(3, 3) = 1.0f;
		}
	};
}