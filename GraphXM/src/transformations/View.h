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
		/* Returns a view matrix representing a camera at CameraPosition, focussed at LookAtPoint with UpVector as up direction (This matrix is considers the right handed coordinate system i.e. outward is +ve z-axis) */
		static Matrix4 LookAt(const Vector3& CameraPosition, const Vector3& LookAtPoint, const Vector3& UpVector);
	};
}