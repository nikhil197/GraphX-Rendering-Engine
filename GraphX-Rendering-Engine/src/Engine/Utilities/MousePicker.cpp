#include "pch.h"
#include "MousePicker.h"

#include "Entities/Camera.h"
#include "Input/Mouse.h"

#include "Engine/Controllers/CameraController.h"

namespace GraphX
{
	std::shared_ptr<MousePicker> MousePicker::s_MousePicker = nullptr;

	void MousePicker::Init(const Ref<Camera>& camera, int width, int height)
	{
		s_MousePicker.reset(new MousePicker(camera, width, height));
	}

	void MousePicker::SetNewCamera(const Ref<Camera>& NewCamera)
	{
		s_MousePicker->m_Camera = NewCamera;
	}

	MousePicker::MousePicker(const Ref<Camera>& camera, int width, int height)
		: m_PickerRay(GM::Vector3::ZeroVector), m_Camera(camera), m_Width(width), m_Height(height)
	{}

	const GM::Vector3& MousePicker::GetPickerRay()
	{
		const GM::Vector2& MousePosition = Mouse::GetMouse()->GetPosition();
		GM::Vector2 NormalCoords;
		ToNormalScreenCoords(MousePosition, NormalCoords);

		GM::Vector3 WorldCoords;
		ToWorldCoords(NormalCoords, WorldCoords);

		m_PickerRay = WorldCoords - m_Camera->GetPosition();
		m_PickerRay.Normalize();

		return m_PickerRay;
	}

	/* Converts the given screen coordinates to normalised screen coordinates */
	void MousePicker::ToNormalScreenCoords(const GM::Vector2& ScreenCoords, GM::Vector2& NormalCoords)
	{
		NormalCoords.x = (2.0f * ScreenCoords.x) / m_Width - 1.0f;
		NormalCoords.y = (2.0f * ScreenCoords.y) / m_Height - 1.0f;
	}

	/* Convertes the given normalised screen coordinates to world coordinates */
	void MousePicker::ToWorldCoords(const GM::Vector2& NormalCoords, GM::Vector3& WorldCoords)
	{
		/** Convert the coordinates from screen space to the world space
			 Inverse Projection Mat to convert to view space
			 Inverse View Mat to convert from view space to world space
		*/																						/* Clip Space Coordinates */
		GM::Vector4 TempWorldCoords = m_Camera->GetProjectionViewMatrix().Inverse() * GM::Vector4(NormalCoords.x, -NormalCoords.y, -1.0f, 1.0f);;

		// Perspective division to get the 3D point
		WorldCoords.x = TempWorldCoords.x / TempWorldCoords.w;
		WorldCoords.y = TempWorldCoords.y / TempWorldCoords.w;
		WorldCoords.z = TempWorldCoords.z / TempWorldCoords.w;
	}
}