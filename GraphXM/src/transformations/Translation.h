#pragma once

namespace gm
{
	//Forward Declarations
	struct Vector2;
	struct Vector3;

	class Matrix3;
	class Matrix4;

	class Translation
	{
	private:
		// To make sure that no instance of the class is created
		Translation();

	public:
		/* Returns a 4x4 matrix obtained after translating mat with given translation vector */
		static Matrix4 Translate(const Matrix4& Mat, const Vector3& Vec);

		/* Returns a 4x4 matrix obtained after translating mat with given Value */
		static Matrix4 Translate(const Matrix4& Mat, float Value);

		/* Returns a 4x4 matrix obtained after translating mat with given translation vector */
		static Matrix3 Translate(const Matrix3& Mat, const Vector2& Vec);
		
		/* Returns a 3x3 matrix obtained after translating mat with given Value */
		static Matrix3 Translate(const Matrix3& Mat, float Value);

		/* Returns a 4x4 translation matrix representing the linear translation given by vec */
		static Matrix4 GetTranslationMatrix(const Vector3& Vec);

		/* Returns a 3x3 translation matrix representing the linear translation given by vec */
		static Matrix3 GetTranslationMatrix(const Vector2& Vec);
	};
}