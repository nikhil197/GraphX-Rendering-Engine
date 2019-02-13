#include "pch.h"
#include "Camera.h"

#include "Timer/Clock.h"

/* Input */
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace engine
{
	Camera::Camera(const gm::Vector3& CameraPos, const gm::Vector3& LookAtPoint, const gm::Vector3& UpAxis, float AspectRatio, float Near, float Far, float FOV)
		:m_CameraLookAtPoint(LookAtPoint), m_WorldUpAxis(UpAxis), m_AspectRatio(AspectRatio), m_Near(Near), m_Far(Far), m_RenderStateDirty(true), m_CameraSpeed(5.5f), CameraPosition(CameraPos), EulerAngles(0), m_ViewAxis(0), m_RightAxis(0), m_UpAxis(0), FieldOfView(FOV)
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
				m_CameraLookAtPoint += CurrentCameraSpeed * m_ViewAxis;
			}
			else if (keyboard->GetKey(Keys::GX_S))
			{
				CameraPosition -= CurrentCameraSpeed * m_ViewAxis;
				m_CameraLookAtPoint -= CurrentCameraSpeed * m_ViewAxis;
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
			else if (keyboard->GetKey(Keys::GX_E))
			{
				CameraPosition += CurrentCameraSpeed * m_UpAxis;
				m_CameraLookAtPoint += CurrentCameraSpeed * m_UpAxis;
			}
			else if (keyboard->GetKey(Keys::GX_Q))
			{
				CameraPosition -= CurrentCameraSpeed * m_UpAxis;
				m_CameraLookAtPoint -= CurrentCameraSpeed * m_UpAxis;
			}
			else
			{
				m_RenderStateDirty = false;
			}
		}
	}

	void Camera::ProcessMouseInput(float DeltaTime)
	{
		if (Mouse::GetMouse()->IsRightButtonPressed())
		{
			const std::shared_ptr<Mouse>& mouse = Mouse::GetMouse();
			const gm::Vector2 LastPosition = mouse->GetLastPosition();
			const gm::Vector2 CurrentPosition = mouse->GetPosition();

			// Calculate the Yaw and the Pitch offset
			float xOffset = CurrentPosition.x - LastPosition.x;
			float yOffset = CurrentPosition.y - LastPosition.y;
			
			if (xOffset != 0 || yOffset != 0)
			{
				m_RenderStateDirty = true;

				float distance = (CameraPosition - gm::Vector3(LastPosition, (CameraPosition.z == 0.0f ? -1.0f : 0.0f))).Magnitude();
				float Yaw = gm::MathUtil::TanInverse(xOffset / distance);
				float Pitch = gm::MathUtil::TanInverse(yOffset / distance);

				gm::Vector3 Angles(-Pitch, -Yaw, 0);
				EulerAngles += Angles;
				gm::MathUtil::ClampAngle(EulerAngles.y);
				gm::MathUtil::ClampAngle(EulerAngles.z);
				gm::MathUtil::Clamp(EulerAngles.x, -90.0f, 90.0f);

				gm::Translation trans(-CameraPosition);
				gm::Matrix4 rotation = gm::Rotation(Angles);

				m_CameraLookAtPoint = gm::Vector3(trans.Inverse() * rotation * trans * gm::Vector4(m_CameraLookAtPoint, 1.0f));
				
				// Update the axes
				UpdateCameraAxes();
			}
		}

		// Check for the mouse scroll
		if (Mouse::GetMouse()->GetScrollOffset() != gm::Vector2::ZeroVector)
		{
			gm::Vector2 Offset = Mouse::GetMouse()->GetScrollOffset();
			FieldOfView -= Offset.y;

			m_RenderStateDirty = true;
		}
	}
}