#include "View.h"

#include "../matrices/Matrix4.h"

#include "../vectors/Vector3.h"

namespace gm
{
	Matrix4 View::LookAt(const Vector3& CameraPosition, const Vector3& LookAtPoint, const Vector3& UpVector)
	{
		Matrix4 result;
		
		// New z - axis after the camera transformations
		Vector3 newZ = (CameraPosition - LookAtPoint).Normal();

		// New x - axis after the camera transformations
		Vector3 newX = Vector3::CrossProduct(UpVector, newZ);
		newX = newX.Normal();

		// UpVector is the newY
		Vector3 newY = UpVector.Normal();

		result[0][0] = newX.x;
		result[1][0] = newX.y;
		result[2][0] = newX.z;

		result[0][1] = newY.x;
		result[1][1] = newY.y;
		result[2][1] = newY.z;

		result[0][2] = newZ.x;
		result[1][2] = newZ.y;
		result[2][2] = newZ.z;

		return result.Inverse();
	}
}