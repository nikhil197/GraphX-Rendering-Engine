#include "GMPch.h"
#include "RotationMatrix.h"

#include "MathUtility.h"

namespace GM
{
	RotationMatrix::RotationMatrix(float Value)
		:Matrix4(),
		m_Angles(Vector3(Value))
	{
		M[0][0] =  (float)Utility::Cos(Value);
		M[0][1] = -(float)Utility::Sin(Value);
		M[1][0] =  (float)Utility::Sin(Value);
		M[1][1] =  (float)Utility::Cos(Value);
	}

	RotationMatrix::RotationMatrix(const Vector3& Angles)
		: Matrix4(),
		m_Angles(Angles)
	{
		/*  OPTIMISATION: Preventing Matrix multiplications
		
		Matrix4 resultX;
		Matrix4 resultY;
		Matrix4 resultZ;

		// Rotating about x - axis
		resultX(1, 1) =  (float)Utility::Cos(Angles.x);
		resultX(2, 2) =  (float)Utility::Cos(Angles.x);
		resultX(1, 2) = -(float)Utility::Sin(Angles.x);
		resultX(2, 1) =  (float)Utility::Sin(Angles.x);

		// Rotating about y - axis
		resultY(0, 0) =  (float)Utility::Cos(Angles.y);
		resultY(0, 2) =  (float)Utility::Sin(Angles.y);
		resultY(2, 2) =  (float)Utility::Cos(Angles.y);
		resultY(2, 0) = -(float)Utility::Sin(Angles.y);

		// Rotating about z - axis
		resultZ(0, 0) =  (float)Utility::Cos(Angles.z);
		resultZ(0, 1) = -(float)Utility::Sin(Angles.z);
		resultZ(1, 0) =  (float)Utility::Sin(Angles.z);
		resultZ(1, 1) =  (float)Utility::Cos(Angles.z);

		*this = resultZ * resultY * resultX;
		*/

		/*
		* A - Angle to be rotated about x - axis
		* B - Angle to be rotated about y - axis
		* C - Angle to be rotated about z - axis
		*/

		float cosA = (float)Utility::Cos(Angles.x);
		float cosB = (float)Utility::Cos(Angles.y);
		float cosC = (float)Utility::Cos(Angles.z);

		float sinA = (float)Utility::Sin(Angles.x);
		float sinB = (float)Utility::Sin(Angles.y);
		float sinC = (float)Utility::Sin(Angles.z);

		M[0][0] = cosB * cosC;
		M[0][1] = sinA * sinB * cosC - cosA * sinC;
		M[0][2] = cosA * sinB * cosC + sinA * sinC;
		M[0][3] = 0.0f;

		M[1][0] = cosB * sinC;
		M[1][1] = sinA * sinB * sinC + cosA * cosC;
		M[1][2] = cosA * sinB * sinC - sinA * cosC;
		M[1][3] = 0.0f;

		M[2][0] = -sinB;
		M[2][1] = sinA * cosB;
		M[2][2] = cosA * cosB;
		M[2][3] = 0.0f;

		M[3][0] = 0.0f;
		M[3][1] = 0.0f;
		M[3][2] = 0.0f;
		M[3][3] = 1.0f;
	}

	RotationMatrix::RotationMatrix(float Angle, const Vector3& Axis)
	{
		// Get the normalized Axis
		Vector3 nAxis = Axis.Normal();

		/*  OPTIMISATION: Preventing Matrix multiplications
		*
		// Get the projection of Axis on the XZ - plane and the YZ - plane
		Vector3 ProjXY = l_Axis * Vector3(1, 1, 0);
		Vector3 ProjXZ = l_Axis * Vector3(1, 0, 1);

		float MagXY = ProjXY.Magnitude();
		float MagXZ = ProjXZ.Magnitude();

		// Calculate the Roll (Rotation about z-axis) and the Yaw (Rotation about y-axis) for the axis
		float Roll = MagXY == 0 ? 0.0f : (float)Utility::ACos(Vector3::DotProduct(ProjXY, Vector3::XAxis) / MagXY);
		float Yaw  = MagXZ == 0 ? 0.0f : (float)Utility::ACos(Vector3::DotProduct(ProjXZ, Vector3::XAxis) / MagXZ);

		// Rotate the axes to make the given axis as the new X - axis
		Rotation rotate(Vector3(0, Yaw, Roll));

		// Rotate the matrix about this new X - Axis;
		M[1][1] =  (float)Utility::Cos(Angle);
		M[2][2] =  (float)Utility::Cos(Angle);
		M[1][2] = -(float)Utility::Sin(Angle);
		M[2][1] =  (float)Utility::Sin(Angle);

		// Rotate the axes back to the original orientation
		*this = rotate.Inverse() * (*this) * rotate;
		*/

		float cos = (float)Utility::Cos(Angle);
		float sin = (float)Utility::Sin(Angle);
		float oneMinusCos = 1 - cos;

		float xy = nAxis.x * nAxis.y;
		float yz = nAxis.y * nAxis.z;
		float zx = nAxis.z * nAxis.x;

		M[0][0] = cos + (nAxis.x * nAxis.x) * oneMinusCos;
		M[0][1] = xy * oneMinusCos - nAxis.z * sin;
		M[0][2] = zx * oneMinusCos + nAxis.y * sin;
		M[0][3] = 0.0f;

		M[1][0] = xy * oneMinusCos + nAxis.z * sin;
		M[1][1] = cos + (nAxis.y * nAxis.y) * oneMinusCos;
		M[1][2] = yz * oneMinusCos - nAxis.x * sin;
		M[1][3] = 0.0f;

		M[2][0] = zx * oneMinusCos - nAxis.y * sin;
		M[2][1] = yz * oneMinusCos + nAxis.x * sin;
		M[2][2] = cos + (nAxis.z * nAxis.z) * oneMinusCos;
		M[2][3] = 0.0f;

		M[3][0] = 0.0f;
		M[3][1] = 0.0f;
		M[3][2] = 0.0f;
		M[3][3] = 1.0f;
	}

	const RotationMatrix& RotationMatrix::operator=(const Matrix4& OtherMat)
	{
		Matrix4::operator=(OtherMat);
		return *this;
	}

	Matrix4 RotationMatrix::Inverse() const
	{
		// For rotation the inverse is the transpose of the matrix
		return Transpose();
	}

	/* Static Members */
	Matrix4 RotationMatrix::Rotate(const Matrix4& Mat, const Vector3& Angles)
	{
		RotationMatrix result(Angles);

		return Mat * result;
	}

	Matrix4 RotationMatrix::Rotate(const Matrix4& Mat, float Angle, const Vector3& Axis)
	{
		return Mat * RotationMatrix(Angle, Axis);
	}

	Matrix4 RotationMatrix::Rotate(const Matrix4& Mat, float Value)
	{
		RotationMatrix result(Value);

		return Mat * result;
	}
}