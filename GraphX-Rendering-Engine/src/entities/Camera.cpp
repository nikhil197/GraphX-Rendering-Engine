#include "pch.h"
#include "Camera.h"

#include "Timer/Clock.h"

/* Input */
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace engine
{
	Camera::Camera(const gm::Vector3& CameraPos, const gm::Vector3& LookAtPoint, const gm::Vector3& UpAxis, float AspectRatio, float Near, float Far, float FOV)
		:m_CameraLookAtPoint(LookAtPoint), m_WorldUpAxis(UpAxis), m_AspectRatio(AspectRatio), m_Near(Near), m_Far(Far), m_RenderStateDirty(true), m_CameraSpeed(2.5f), CameraPosition(CameraPos), m_ViewAxis(0), m_RightAxis(0), m_UpAxis(0), FieldOfView(FOV)
	{
		UpdateCameraAxes();
	}

	void Camera::UpdateCameraAxes()
	{
		m_ViewAxis = CameraPosition - m_CameraLookAtPoint;
		m_ViewAxis.Normalize();

		m_RightAxis = gm::Vector3::CrossProduct(m_WorldUpAxis, m_ViewAxis);
		m_RightAxis.Normalize();

		m_UpAxis = gm::Vector3::CrossProduct(m_ViewAxis, m_RightAxis);
	}

	void Camera::Update(float DeltaTime)
	{
		ProcessKeyboardInput(DeltaTime);
		ProcessMouseInput(DeltaTime);
	}

	void Camera::ProcessKeyboardInput(float DeltaTime)
	{
		// Update the camera position
		if (Mouse::GetMouse()->IsRightButtonPressed())
		{
			float CurrentCameraSpeed = m_CameraSpeed * DeltaTime;
			const std::shared_ptr<Keyboard>& keyboard = Keyboard::GetKeyboard();
			m_RenderStateDirty = true;
			
			if (keyboard->GetKey(Keys::GX_W))
			{
				CameraPosition += CurrentCameraSpeed * m_ViewAxis;
			}
			else if (keyboard->GetKey(Keys::GX_S))
			{
				CameraPosition -= CurrentCameraSpeed * m_ViewAxis;
			}
			else if (keyboard->GetKey(Keys::GX_A))
			{
				CameraPosition -= CurrentCameraSpeed * m_RightAxis;
				m_CameraLookAtPoint -= CurrentCameraSpeed * m_RightAxis;
			}
			else if (keyboard->GetKey(Keys::GX_D))
			{
				CameraPosition += CurrentCameraSpeed * m_RightAxis;
				m_CameraLookAtPoint += CurrentCameraSpeed * m_RightAxis;
			}
			else
			{
				m_RenderStateDirty = false;
			}
		}
	}

	void Camera::ProcessMouseInput(float DeltaTime)
	{

	}
}