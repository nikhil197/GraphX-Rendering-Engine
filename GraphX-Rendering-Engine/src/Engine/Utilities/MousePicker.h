#pragma once

namespace GraphX
{
	class MousePicker
	{
	public:
		static void Init(const Ref<class Camera>& camera, int width, int height);

		static void SetNewCamera(const Ref<class Camera>& NewCamera);

		static const std::shared_ptr<MousePicker>& Get() { return s_MousePicker; }

	private:
		MousePicker(const Ref<class Camera>& camera, int width, int height);

	public:
		/* Converts the given screen coordinates to normalised screen coordinates */
		void ToNormalScreenCoords(const GM::Vector2& ScreenCoords, /* out */GM::Vector2& NormalCoords);

		/* Convertes the given normalised screen coordinates to world coordinates */
		void ToWorldCoords(const GM::Vector2& NormalCoords, /* out */GM::Vector3& WorldCoords);

		/* Returns the picker ray */
		const GM::Vector3& GetPickerRay();

	private:
		/* Ray used to pick the intersecting object in the scene */
		GM::Vector3 m_PickerRay;

		/* Main Camera of the renderer */
		Ref<class Camera> m_Camera;

		/* Width and Height of the Main window of the renderer */
		int m_Width, m_Height;

		/* Static instance for the mouse picker */
		static std::shared_ptr<MousePicker> s_MousePicker;
	};;
}