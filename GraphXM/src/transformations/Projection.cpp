#include "Projection.h"

#include "../matrices/Matrix4.h"

#include "../vectors/Vector3.h"

#include "Translation.h"
#include "Scaling.h"

namespace gm
{
	Matrix4 Projection::Ortho(float top, float left, float bottom, float right, float near, float far)
	{
		float rml = right - left;
		float tmb = top - bottom;
		float fmn = far - near;

		Vector3 TranslationVector;
		TranslationVector.x = (right == left) ? 0.0f : -((right + left) / rml);
		TranslationVector.y = (top == bottom) ? 0.0f : -((top + bottom) / tmb);
		TranslationVector.z = (near == far) ? 0.0f : -((far + near) / fmn);

		Vector3 ScaleVector;
		ScaleVector.x = (right == left) ? right : 2 / rml;
		ScaleVector.y = (top == bottom) ? top : 2 / tmb;
		ScaleVector.z = (far == near) ? far : 2 / fmn;

		Translation trans(TranslationVector);

		Scaling scale(ScaleVector);

		return trans * scale;
	}
}