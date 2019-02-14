#include "pch.h"
#include "MousePicker.h"

#include "Entities/Camera.h"
#include "Input/Mouse.h"

namespace engine
{
	MousePicker::MousePicker(const Camera& camera, float width, float height)
		: m_PickerRay(gm::Vector3::ZeroVector), m_Camera(camera), m_Width(width), m_Height(height)
	{}

	void MousePicker::Update()
	{
		const gm::Vector2& MousePosition = Mouse::GetMouse()->GetPosition();
		gm::Vector3 WorldCoords = ToWorldCoordinates(ToNormalisedScreenCoordinates(MousePosition));

		// Update the ray
		m_PickerRay = WorldCoords - m_Camera.CameraPosition;
	}

	gm::Vector2 MousePicker::ToNormalisedScreenCoordinates(const gm::Vector2& ScreenCoords)
	{
		gm::Vector2 NormalisedCoords;
		NormalisedCoords.x = (2.0f * ScreenCoords.x) / m_Width - 1.0f;
		NormalisedCoords.y = 1.0f - (2.0f * ScreenCoords.y) / m_Height;

		return NormalisedCoords;
	}

	gm::Vector3 MousePicker::ToWorldCoordinates(const gm::Vector2& NormalisedCoords)
	{
		// Coordinates in the view space													/* Clip Space Coordinates */
		gm::Vector4 ViewSpaceCoords = m_Camera.GetPerspectiveProjectionMatrix().Inverse() * gm::Vector4(NormalisedCoords, -1.0f, 1.0f);

		gm::Vector4 WorldCoords = m_Camera.GetViewMatrix().Inverse() * ViewSpaceCoords;

		return gm::Vector3(WorldCoords.x, WorldCoords.y, WorldCoords.z);
	}
}