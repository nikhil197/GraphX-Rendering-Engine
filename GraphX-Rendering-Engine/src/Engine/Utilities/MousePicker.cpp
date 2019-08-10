#include "pch.h"
#include "MousePicker.h"

#include "Entities/Camera.h"
#include "Input/Mouse.h"

namespace GraphX
{
	MousePicker::MousePicker(const Camera& camera, float width, float height)
		: m_PickerRay(GraphXMaths::Vector3::ZeroVector), m_Camera(camera), m_Width(width), m_Height(height)
	{}

	void MousePicker::Update()
	{
		const GraphXMaths::Vector2& MousePosition = Mouse::GetMouse()->GetPosition();
		GraphXMaths::Vector3 WorldCoords = ToWorldCoordinates(ToNormalisedScreenCoordinates(MousePosition));

		// Update the ray
		m_PickerRay = (WorldCoords - m_Camera.CameraPosition).Normal();


	}

	GraphXMaths::Vector2 MousePicker::ToNormalisedScreenCoordinates(const GraphXMaths::Vector2& ScreenCoords)
	{
		GraphXMaths::Vector2 NormalisedCoords;
		NormalisedCoords.x = (2.0f * ScreenCoords.x) / m_Width - 1.0f;
		NormalisedCoords.y = 1.0f - (2.0f * ScreenCoords.y) / m_Height;

		return NormalisedCoords;
	}

	GraphXMaths::Vector3 MousePicker::ToWorldCoordinates(const GraphXMaths::Vector2& NormalisedCoords)
	{
		// Coordinates in the view space													/* Clip Space Coordinates */
		GraphXMaths::Vector4 ViewSpaceCoords = m_Camera.GetProjectionMatrix().Inverse() * GraphXMaths::Vector4(NormalisedCoords, -1.0f, 1.0f);

		GraphXMaths::Vector4 WorldCoords = m_Camera.GetViewMatrix().Inverse() * ViewSpaceCoords;

		return GraphXMaths::Vector3(WorldCoords.x, WorldCoords.y, WorldCoords.z);
	}
}