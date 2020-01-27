#pragma once

namespace GraphX
{
	class MousePicker
	{
	public:
		MousePicker(const class Camera& camera, float width, float height);

		/* Updates the casting ray of the mouse picker */
		void Update();

		/* Converts the given screen coordinates to normalised screen coordinates */
		GM::Vector2 ToNormalisedScreenCoordinates(const GM::Vector2& ScreenCoords);

		/* Convertes the given normalised screen coordinates to world coordinates */
		GM::Vector3 ToWorldCoordinates(const GM::Vector2& NormalisedCoords);

		/* Returns the picker ray */
		inline const GM::Vector3& GetPickerRay() const { return m_PickerRay; }

	private:
		/* Ray used to pick the intersecting object in the scene */
		GM::Vector3 m_PickerRay;

		/* Main Camera of the renderer */
		const class Camera& m_Camera;

		/* Width and Height of the Main window of the renderer */
		float m_Width, m_Height;
	};;
}