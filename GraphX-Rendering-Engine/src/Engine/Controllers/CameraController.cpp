#include "pch.h"
#include "CameraController.h"

#include "Engine/Entities/Camera.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/WindowEvent.h"

#include "Engine/Input/Mouse.h"
#include "Engine/Input/Keyboard.h"

namespace GraphX
{
	//TODO: Zoom

	CameraController::CameraController(const GM::Vector3& CameraPos, const GM::Vector3& LookAtPoint, const GM::Vector3& UpAxis, float AspectRatio, float Near, float Far, float FOV)
		: m_ViewAxis(0), m_RightAxis(0), m_UpAxis(UpAxis), m_AspectRatio(AspectRatio), m_NearClipPlane(Near), m_FarClipPlane(Far), m_FieldOfView(FOV), m_Camera(CreateRef<Camera>(CameraPos, LookAtPoint))
	{
		m_ViewAxis = LookAtPoint - m_Camera->m_Position;
		m_ViewAxis.Normalize();

		m_RightAxis = GM::Vector3::CrossProduct(m_ViewAxis, m_UpAxis);
		m_RightAxis.Normalize();

		m_Camera->m_Controller = this;
	}

	CameraController::~CameraController()
	{
	}

	void CameraController::Update(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		// Previous update has already been processed by the application (saves one function call)
		m_Camera->m_RenderStateDirty = false;

		ProcessKeyboardInput(DeltaTime);
		ProcessMouseInput(DeltaTime);

		UpdateProjectionViewMatrix();
	}

	void CameraController::ProcessKeyboardInput(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		// Update the camera position
		if (Mouse::GetMouse()->IsRightButtonPressed())
		{
			float CurrentCameraSpeed = TranslationSpeed * DeltaTime * m_ZoomLevel;
			const std::shared_ptr<Keyboard>& keyboard = Keyboard::GetKeyboard();

			if (m_CurrentProjectionMode == ProjectionMode::Perspective)
			{
				if (keyboard->GetKey(Keys::GX_W))
				{
					m_Camera->m_Position += CurrentCameraSpeed * m_ViewAxis;
					m_ViewChanged = true;
				}
				else if (keyboard->GetKey(Keys::GX_S))
				{
					m_Camera->m_Position -= CurrentCameraSpeed * m_ViewAxis;
					m_ViewChanged = true;
				}

				if (keyboard->GetKey(Keys::GX_A))
				{
					m_Camera->m_Position -= CurrentCameraSpeed * m_RightAxis;
					m_ViewChanged = true;
				}
				else if (keyboard->GetKey(Keys::GX_D))
				{
					m_Camera->m_Position += CurrentCameraSpeed * m_RightAxis;
					m_ViewChanged = true;
				}

				if (keyboard->GetKey(Keys::GX_E))
				{
					m_Camera->m_Position += CurrentCameraSpeed * m_UpAxis;
					m_ViewChanged = true;
				}
				else if (keyboard->GetKey(Keys::GX_Q))
				{
					m_Camera->m_Position -= CurrentCameraSpeed * m_UpAxis;
					m_ViewChanged = true;
				}
			}
			else
			{
				if (keyboard->GetKey(Keys::GX_W))
				{
					m_Camera->m_Position += CurrentCameraSpeed * EngineConstants::UpAxis;
					m_ViewChanged = true;
				}
				else if (keyboard->GetKey(Keys::GX_S))
				{
					m_Camera->m_Position -= CurrentCameraSpeed * EngineConstants::UpAxis;
					m_ViewChanged = true;
				}

				if (keyboard->GetKey(Keys::GX_A))
				{
					m_Camera->m_Position -= CurrentCameraSpeed * EngineConstants::RightAxis;
					m_ViewChanged = true;
				}
				else if (keyboard->GetKey(Keys::GX_D))
				{
					m_Camera->m_Position += CurrentCameraSpeed * EngineConstants::RightAxis;
					m_ViewChanged = true;
				}
			}
		}
	}

	void CameraController::ProcessMouseInput(float DeltaTime)
	{
		GX_PROFILE_FUNCTION()

		if (m_CurrentProjectionMode == ProjectionMode::Perspective)
		{
			// Calculate the Yaw and the Pitch offset
			const std::shared_ptr<Mouse>& mouse = Mouse::GetMouse();
			float xOffset = mouse->GetPositionDelta().x;	// Yaw
			float yOffset = mouse->GetPositionDelta().y;	// Pitch
			
			float OrbitSensitivity = 10.0f;
			DeltaTime *= OrbitSensitivity;

			xOffset *= DeltaTime;
			yOffset *= DeltaTime;

			GM::Utility::Clamp(xOffset, -10.0f, 10.0f);
			GM::Utility::Clamp(yOffset, -10.0f, 10.0f);

			if (xOffset != 0.0f || yOffset != 0.0f)
			{
				// Orbit around the current location
				if (mouse->IsRightButtonPressed() && mouse->IsMouseDragged())
				{
					UpdateCameraOrientation(xOffset, yOffset);
				}
				else if (mouse->IsLeftButtonPressed() && mouse->IsMouseDragged())
				{
					// TODO: Orbit and move
					UpdateCameraOrientation(xOffset, 0);

					float CurrentCameraSpeed = TranslationSpeed * DeltaTime * m_ZoomLevel;
					m_Camera->m_Position -= yOffset * CurrentCameraSpeed * m_ViewAxis;
				}
			}
		}
	}

	void CameraController::UpdateProjectionViewMatrix()
	{
		GX_PROFILE_FUNCTION()

		if (m_CurrentProjectionMode == ProjectionMode::Perspective)
		{
			if (m_ProjDataChanged)
			{
				GM::ProjectionMatrix::Perspective(m_Camera->m_ProjectionMatrix, m_FieldOfView * m_ZoomLevel, m_AspectRatio, m_NearClipPlane, m_FarClipPlane);
			}

			if (m_ViewChanged)
			{
				GM::ViewMatrix::LookAt(m_Camera->m_ViewMatrix, m_Camera->m_Position, m_ViewAxis, m_RightAxis, m_UpAxis);
				m_Camera->m_RotationViewMatrix = m_Camera->m_ViewMatrix;
				m_Camera->m_RotationViewMatrix(0, 3) = 0.0f;
				m_Camera->m_RotationViewMatrix(1, 3) = 0.0f;
				m_Camera->m_RotationViewMatrix(2, 3) = 0.0f;
			}
		}
		else if (m_CurrentProjectionMode == ProjectionMode::Orthographic)
		{
			if (m_ProjDataChanged)
			{
				float HalfOrthoWidth = m_OrthoWidth * m_ZoomLevel / 2.0f;
				float HalfOrthoHeight = m_OrthoHeight * m_ZoomLevel / 2.0f;
				GM::ProjectionMatrix::Ortho(m_Camera->m_ProjectionMatrix, -HalfOrthoWidth, HalfOrthoWidth, -HalfOrthoHeight, HalfOrthoHeight, m_NearClipPlane, m_FarClipPlane);
			}

			if (m_ViewChanged)
			{
				GM::ViewMatrix::LookAt(m_Camera->m_ViewMatrix, m_Camera->m_Position, m_Camera->m_Position + GM::Vector3(0.f, 0.f, -3.0f), GM::Vector3::YAxis);
				m_Camera->m_RotationViewMatrix = m_Camera->m_ViewMatrix;
				m_Camera->m_RotationViewMatrix(0, 3) = 0.0f;
				m_Camera->m_RotationViewMatrix(1, 3) = 0.0f;
				m_Camera->m_RotationViewMatrix(2, 3) = 0.0f;
			}
		}

		// Change the Projection view matrix if any change has been made to the projection and view matrices
		if (m_ProjDataChanged || m_ViewChanged)
		{
			m_Camera->m_ProjectionViewMatrix = m_Camera->m_ProjectionMatrix * m_Camera->m_ViewMatrix;
			m_Camera->m_RenderStateDirty = true;
			m_ProjDataChanged = false;
			m_ViewChanged = false;
		}
	}

	void CameraController::UpdateCameraOrientation(float xOffset, float yOffset)
	{
		m_ViewChanged = true;

		m_Camera->m_Rotation.Pitch -= yOffset;
		m_Camera->m_Rotation.Yaw -= xOffset;
		
		//TODO: Also change the roll
		m_Camera->m_Rotation.Pitch = GM::Utility::ClampAngle(m_Camera->m_Rotation.Pitch, -89.9f, 89.9f);
		m_Camera->m_Rotation.Yaw = GM::Utility::ClampAngle(m_Camera->m_Rotation.Yaw, 0.0f, 359.999f);

		GM::Quat RotationQuat(m_Camera->m_Rotation);
		m_RightAxis = RotationQuat * EngineConstants::RightAxis;
		m_ViewAxis = RotationQuat * EngineConstants::ForwardAxis;
		m_UpAxis = RotationQuat * EngineConstants::UpAxis;
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(CameraController::OnMouseScroll));
		dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUNC(CameraController::OnViewPortResize));
	}

	const GM::Vector3& CameraController::GetCameraPosition() const
	{ 
		return m_Camera->m_Position; 
	}

	void CameraController::SetCameraPosition(const GM::Vector3& NewPosition)
	{
		if (NewPosition != m_Camera->m_Position)
		{
			m_Camera->m_Position = NewPosition;
			m_ViewChanged = true;
			UpdateProjectionViewMatrix();
		}
	}

	void CameraController::SetCameraOrientation(const GM::Rotator& NewOrientation)
	{
		if (NewOrientation != m_Camera->m_Rotation)
		{
			m_Camera->m_Rotation = NewOrientation;

			UpdateCameraOrientation(0, 0);

			UpdateProjectionViewMatrix();
		}
	}

	const GM::Rotator& CameraController::GetCameraOrientation() const
	{ 
		return m_Camera->m_Rotation;
	}

	void CameraController::SetAspectRatio(float NewRatio)
	{
		if (m_AspectRatio != NewRatio)
		{
			m_AspectRatio = NewRatio;
			m_ProjDataChanged = true;
			UpdateProjectionViewMatrix();
		}
	}

	void CameraController::SetNearClipPlane(float NewNear)
	{
		if (m_NearClipPlane != NewNear)
		{
			m_NearClipPlane = NewNear;
			m_ProjDataChanged = true;
			UpdateProjectionViewMatrix();
		}
	}

	void CameraController::SetFarClipPlane(float NewFar)
	{
		if (m_FarClipPlane != NewFar)
		{
			m_FarClipPlane = NewFar;
			m_ProjDataChanged = true;
			UpdateProjectionViewMatrix();
		}
	}

	void CameraController::SetProjectionMode(ProjectionMode NewMode)
	{
		if (m_CurrentProjectionMode != NewMode)
		{
			m_CurrentProjectionMode = NewMode;
			m_ProjDataChanged = true;
			m_ViewChanged = true;
			m_Camera->m_ProjectionMatrix.Identity();
			m_Camera->m_ViewMatrix.Identity();
			UpdateProjectionViewMatrix();
		}
	}

	void CameraController::SetFieldOfView(float NewFOV)
	{
		if (m_FieldOfView != NewFOV)
		{
			m_FieldOfView = NewFOV;
			m_ProjDataChanged = true;
			UpdateProjectionViewMatrix();
		}
	}

	void CameraController::SetOrthoWidth(float NewWidth)
	{
		if (m_OrthoWidth != NewWidth)
		{
			m_OrthoWidth = NewWidth;
			m_ProjDataChanged = true;
			UpdateProjectionViewMatrix();
		}
	}

	void CameraController::SetOrthoHeight(float NewHeight)
	{
		if (m_OrthoHeight != NewHeight)
		{
			m_OrthoHeight = NewHeight;
			m_ProjDataChanged = true;
			UpdateProjectionViewMatrix();
		}
	}

	bool CameraController::OnViewPortResize(WindowResizedEvent& e)
	{
		GX_PROFILE_FUNCTION()

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_OrthoWidth = (float)e.GetWidth();
		m_OrthoHeight = (float)e.GetHeight();
		UpdateProjectionViewMatrix();

		// Return false just in case other layers / viewport need to respond to this event as well.
		return false;
	}

	bool CameraController::OnMouseScroll(class MouseScrolledEvent& e)
	{
		GX_PROFILE_FUNCTION()

		m_ZoomLevel -= e.GetYOffset() * .25f * Mouse::GetMouse()->ScrollSenstivity;

		GM::Utility::Clamp(m_ZoomLevel, 0.2f, 16.0f);

		m_ProjDataChanged = true;

		// Return false just in case other layers / viewport need to respond to this event as well.
		return false;
	}
}