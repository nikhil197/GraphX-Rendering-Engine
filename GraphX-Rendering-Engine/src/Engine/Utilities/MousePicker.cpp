#include "pch.h"
#include "MousePicker.h"

#include "Entities/Camera.h"
#include "Input/Mouse.h"

namespace GraphX
{
	MousePicker::MousePicker(const Camera& camera, float width, float height)
		: m_PickerRay(GM::Vector3::ZeroVector), m_Camera(camera), m_Width(width), m_Height(height)
	{}

	void MousePicker::Update()
	{
		const GM::Vector2& MousePosition = Mouse::GetMouse()->GetPosition();
		GM::Vector3 WorldCoords = ToWorldCoordinates(ToNormalisedScreenCoordinates(MousePosition));

		// Update the ray
		m_PickerRay = (WorldCoords - m_Camera.CameraPosition).Normal();


	}

	GM::Vector2 MousePicker::ToNormalisedScreenCoordinates(const GM::Vector2& ScreenCoords)
	{
		GM::Vector2 NormalisedCoords;
		NormalisedCoords.x = (2.0f * ScreenCoords.x) / m_Width - 1.0f;
		NormalisedCoords.y = 1.0f - (2.0f * ScreenCoords.y) / m_Height;

		return NormalisedCoords;
	}

	GM::Vector3 MousePicker::ToWorldCoordinates(const GM::Vector2& NormalisedCoords)
	{
		// Coordinates in the view space													/* Clip Space Coordinates */
		GM::Vector4 ViewSpaceCoords = m_Camera.GetProjectionMatrix().Inverse() * GM::Vector4(NormalisedCoords, -1.0f, 1.0f);

		GM::Vector4 WorldCoords = m_Camera.GetViewMatrix().Inverse() * ViewSpaceCoords;

		return GM::Vector3(WorldCoords.x, WorldCoords.y, WorldCoords.z);
	}
}