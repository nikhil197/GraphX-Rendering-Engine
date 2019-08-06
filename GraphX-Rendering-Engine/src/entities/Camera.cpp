#include "pch.h"
#include "Camera.h"

#include "Timer/Clock.h"

/* Input */
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace GraphX
{
	Camera::Camera(const GraphXMaths::Vector3& CameraPos, const GraphXMaths::Vector3& LookAtPoint, const GraphXMaths::Vector3& UpAxis, float AspectRatio, float Near, float Far, float FOV)
		: m_WorldUpAxis(UpAxis), m_ViewAxis(0), m_RightAxis(0), m_UpAxis(0), m_AspectRatio(AspectRatio), m_Near(Near), m_Far(Far), m_FieldOfView(FOV), CameraPosition(CameraPos), EulerAngles(0)
	{
		m_ViewAxis = LookAtPoint - CameraPosition;
		m_ViewAxis.Normalize();

		m_RightAxis = GraphXMaths::Vector3::CrossProduct(m_ViewAxis, m_WorldUpAxis);
		m_RightAxis.Normalize();

		m_UpAxis = GraphXMaths::Vector3::CrossProduct(m_RightAxis, m_ViewAxis);

		// Calculate the view and projection matrices (Default projection mode is perspective)
		GraphXMaths::View::LookAt(m_ViewMatrix, CameraPosition, m_ViewAxis, m_RightAxis, m_UpAxis);
		CalculateProjectionMatrix();
	}

	void Camera::Update(float DeltaTime)
	{
		
		ProcessKeyboardInput(DeltaTime);
		ProcessMouseInput(DeltaTime);

		if (m_ViewChanged)
		{
			GraphXMaths::View::LookAt(m_ViewMatrix, CameraPosition, m_ViewAxis, m_RightAxis, m_UpAxis);
			m_RenderStateDirty = true;
		}

		if (m_ProjDataChanged)
		{
			CalculateProjectionMatrix();
			m_RenderStateDirty = true;
		}

		if (m_RenderStateDirty)
		{
			m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	}

	void Camera::ProcessKeyboardInput(float DeltaTime)
	{
		// Update the camera position
		if (Mouse::GetMouse()->IsRightButtonPressed())
		{
			float CurrentCameraSpeed = CameraSpeed * DeltaTime;
			const std::shared_ptr<Keyboard>& keyboard = Keyboard::GetKeyboard();
			m_ViewChanged = true;
			
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
			}
			else if (keyboard->GetKey(Keys::GX_D))
			{
				CameraPosition += CurrentCameraSpeed * m_RightAxis;
			}
			else if (keyboard->GetKey(Keys::GX_E))
			{
				CameraPosition += CurrentCameraSpeed * m_UpAxis;
			}
			else if (keyboard->GetKey(Keys::GX_Q))
			{
				CameraPosition -= CurrentCameraSpeed * m_UpAxis;
			}
			else
			{
				m_ViewChanged = false;
			}
		}
	}

	void Camera::ProcessMouseInput(float DeltaTime)
	{
		if (Mouse::GetMouse()->IsRightButtonPressed())
		{
			const std::shared_ptr<Mouse>& mouse = Mouse::GetMouse();
			const GraphXMaths::Vector2& LastPosition = mouse->GetLastPosition();
			const GraphXMaths::Vector2& CurrentPosition = mouse->GetPosition();

			// Calculate the Yaw and the Pitch offset
			float xOffset = CurrentPosition.x - LastPosition.x;
			float yOffset = CurrentPosition.y - LastPosition.y;
			
			if ((xOffset != 0 && GraphXMaths::Abs(xOffset) < 20.0f) || (yOffset != 0 && GraphXMaths::Abs(yOffset) < 20.0f))
			{
				xOffset *= DeltaTime;
				yOffset *= DeltaTime;

				m_ViewChanged = true;

				EulerAngles.x += yOffset;
				EulerAngles.y += xOffset;

				if (EulerAngles.x >= 89.0f)
					EulerAngles.x = 89.0f;
				else if (EulerAngles.x <= -89.0f)
					EulerAngles.x = -89.0f;

				GraphXMaths::ClampAngle(EulerAngles.y);

				m_RightAxis = GraphXMaths::Vector3::CrossProduct(m_ViewAxis, m_WorldUpAxis);
				m_ViewAxis = GraphXMaths::Vector3(GraphXMaths::Rotation(xOffset, m_UpAxis) * GraphXMaths::Rotation(-yOffset, m_RightAxis) * GraphXMaths::Vector4(m_ViewAxis, 1.0f));
				m_UpAxis = GraphXMaths::Vector3::CrossProduct(m_RightAxis, m_ViewAxis);
			}
		}

		// Check for the mouse scroll only in perspective projection mode
		if (m_CurrentProjectionMode == ProjectionMode::Perspective && Mouse::GetMouse()->GetScrollOffset() != GraphXMaths::Vector2::ZeroVector)
		{
			GraphXMaths::Vector2 Offset = Mouse::GetMouse()->GetScrollOffset();
			m_FieldOfView -= Offset.y;

			m_ProjDataChanged = true;
		}
	}

	void Camera::CalculateProjectionMatrix()
	{
		if (m_CurrentProjectionMode == ProjectionMode::Perspective)
		{
			m_ProjectionMatrix = GraphXMaths::Projection::Perspective(m_FieldOfView, m_AspectRatio, m_Near, m_Far);
		}
		else
		{
			float HalfOrthoWidth = m_OrthoWidth / 2.0f;
			float HalfOrthoHeight = m_OrthoHeight / 2.0f;
			m_ProjectionMatrix = GraphXMaths::Projection::Ortho(-HalfOrthoWidth, HalfOrthoWidth, -HalfOrthoHeight, HalfOrthoHeight, m_Near, m_Far);
		}

		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::Enable(class Shader& shader, const std::string& Name) const
	{
	}

	void Camera::Disable() const
	{
	}
}