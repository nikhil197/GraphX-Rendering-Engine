#include "pch.h"
#include "Camera.h"

namespace engine
{
	Camera::Camera(const gm::Vector3& CameraPos, const gm::Vector3& LookAtPoint, const gm::Vector3& UpAxis, float AspectRatio, float Near, float Far, float FOV)
		:m_CameraLookAtPoint(LookAtPoint), m_WorldUpAxis(UpAxis), m_AspectRatio(AspectRatio), m_Near(Near), m_Far(Far), CameraPosition(CameraPos), ViewAxis(0), RightAxis(0), UpAxis(0), FieldOfView(FOV)
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