#include "GMPch.h"
#include "ViewMatrix.h"

#include "Vectors/Vector3.h"
#include "Matrices/Matrix4.h"

namespace GM
{
	Matrix4 ViewMatrix::LookAt(const Vector3& CameraPosition, const Vector3& LookAtPoint, const Vector3& UpVector)
	{
		Matrix4 result;
		
		LookAt(result, CameraPosition, LookAtPoint, UpVector);

		return result;
	}

	Matrix4 ViewMatrix::LookAt(const Vector3& CameraPosition, const Vector3& ViewAxis, const Vector3& RightAxis, const Vector3& UpAxis)
	{
		Matrix4 result;

		LookAt(result, CameraPosition, ViewAxis, RightAxis, UpAxis);

		return result;
	}

	void ViewMatrix::LookAt(Matrix4& Mat, const Vector3& CameraPosition, const Vector3& LookAtPoint, const Vector3& UpVector)
	{
		// New z - axis after the camera transformations (View Vector i.e. from target to camera)
		Vector3 ViewVector = (LookAtPoint - CameraPosition);
		ViewVector.Normalize();

		// New x - axis after the camera transformations (Right vector)
		Vector3 RightVector = Vector3::CrossProduct(ViewVector, UpVector);
		RightVector.Normalize();

		// New y - axis (Final Up Vector)
		Vector3 NewUpVector = Vector3::CrossProduct(RightVector, ViewVector);

		LookAt(Mat, CameraPosition, ViewVector, RightVector, NewUpVector);
	}

	void ViewMatrix::LookAt(Matrix4& Mat, const Vector3& CameraPosition, const Vector3& ViewAxis, const Vector3& RightAxis, const Vector3& UpAxis)
	{
		Mat(0, 0) = RightAxis.x;
		Mat(0, 1) = RightAxis.y;
		Mat(0, 2) = RightAxis.z;

		Mat(1, 0)= UpAxis.x;
		Mat(1, 1)= UpAxis.y;
		Mat(1, 2)= UpAxis.z;

		Mat(2, 0) = -ViewAxis.x;
		Mat(2, 1) = -ViewAxis.y;
		Mat(2, 2) = -ViewAxis.z;

		// Get the projection of the translation vector onto the new axes
		Mat(0, 3) = -Vector3::DotProduct(RightAxis, CameraPosition);
		Mat(1, 3) = -Vector3::DotProduct(UpAxis, CameraPosition);
		Mat(2, 3) = Vector3::DotProduct(ViewAxis, CameraPosition);
	}

}