#pragma once

namespace gm
{
	// Forward declarations
	class Matrix4;

	struct Vector3;

	class View
	{
	private:
		/* Private constructor to make sure no instance of this class is created */
		View();

	public:
		/* Returns a view matrix representing a camera at CameraPosition, focused at LookAtPoint with UpVector as up direction (This matrix is considers the right handed coordinate system i.e. outward is +ve z-axis) */
		static Matrix4 LookAt(const Vector3& CameraPosition, const Vector3& LookAtPoint, const Vector3& UpVector);

		/* Returns a view matrix representing camera at CameraPosition and orientation given by the axes */
		static Matrix4 LookAt(const Vector3& CameraPosition, const Vector3& ViewAxis, const Vector3& RightAxis, const Vector3& UpAxis);

		static void LookAt(Matrix4& Mat, const Vector3& CameraPosition, const Vector3& LookAtPoint, const Vector3& UpVector);

		static void LookAt(Matrix4& Mat, const Vector3& CameraPosition, const Vector3& ViewAxis, const Vector3& RightAxis, const Vector3& UpAxis);

	};
}