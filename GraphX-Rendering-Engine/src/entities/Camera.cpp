#include "pch.h"
#include "Camera.h"

namespace engine
{
	Camera::Camera(const gm::Vector3& CameraPos, const gm::Vector3& LookAtPoint, const gm::Vector3& UpAxis)
		:m_CameraLookAtPoint(LookAtPoint), m_WorldUpAxis(UpAxis), m_AspectRatio(4 / 3), m_Near(0.1f), m_Far(1000.f), CameraPosition(CameraPos), ViewAxis(0), RightAxis(0), UpAxis(0), FieldOfView(45.0f)
	{
		UpdateCameraAxes();
	}

	void Camera::UpdateCameraAxes()
	{
		ViewAxis = CameraPosition - m_CameraLookAtPoint;
		ViewAxis.Normalize();

		RightAxis = gm::Vector3::CrossProduct(m_WorldUpAxis, ViewAxis);
		RightAxis.Normalize();

		UpAxis = gm::Vector3::CrossProduct(ViewAxis, RightAxis);
	}
}