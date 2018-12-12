#pragma once

namespace engine
{
	class Camera
	{
	private:
		/* Point to which the camera is pointing */
		gm::Vector3 m_CameraLookAtPoint;

		/* Up Axis for the whole world */
		gm::Vector3 m_WorldUpAxis;

	public:
		/* Camera Attributes */
		/* Position of the camera */
		gm::Vector3 CameraPosition;

		/* Camera view axis */
		gm::Vector3 ViewAxis;

		/* Right axis in view coordinates */
		gm::Vector3 RightAxis;

		/* Up Axis in view coordinates */
		gm::Vector3 UpAxis;

	public:
		/* Constructor */
		Camera(const gm::Vector3& CameraPos, const gm::Vector3& LookAtPoint, const gm::Vector3& UpAxis);

		/* Updates the camera axis */
		void UpdateCameraAxes();

		/* Returns a view matrix for the camera */
		inline gm::Matrix4 GetViewMatrix() const { return gm::View::LookAt(CameraPosition, m_CameraLookAtPoint, m_WorldUpAxis); }
	};
}