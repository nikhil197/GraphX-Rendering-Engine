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

		/* The aspect ratio of the camera frame */
		float m_AspectRatio;

		/* The z - coordinate of the near plane behind which nothing will be rendered */
		float m_Near;

		/* The z - coordinate of the far plane beyond which nothing will be rendered */
		float m_Far;

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

		/* Field of view of the camera (in degrees) */
		float FieldOfView;

		/* Camera settings for control */
		float CameraSpeed;

	public:
		/* Constructor */
		Camera(const gm::Vector3& CameraPos, const gm::Vector3& LookAtPoint, const gm::Vector3& UpAxis, float AspectRatio = (4.0f / 3.0f), float Near = 0.1f, float Far = 1000.0f, float FOV = 45.0f);

		/* Updates the camera axis */
		void UpdateCameraAxes();

		/* Sets the new aspect ratio for the camera */
		inline void SetAspectRatio(float Ratio) { m_AspectRatio = Ratio; }

		/* Sets the new Near plane for the camera */
		inline void SetNearPlane(float Near) { m_Near = Near; }

		/* Sets the new Far plane for the camera */
		inline void SetFarPlane(float Far) { m_Far = Far; }

		/* Returns a view matrix for the camera */
		inline gm::Matrix4 GetViewMatrix() const { return gm::View::LookAt(CameraPosition, m_CameraLookAtPoint, m_WorldUpAxis); }

		/* Returns the projection matrix for the current camera settings */
		inline gm::Matrix4 GetPerspectiveProjectionMatrix() const { return gm::Projection::Perspective(FieldOfView, m_AspectRatio, m_Near, m_Far); }

		/* Returns the orthographic matrix for the current settings and the provided width and height */
		inline gm::Matrix4 GetOrthographiProjectionMatrix(float width, float height) const { return gm::Projection::Ortho(0, width, 0, height, m_Near, m_Far); }
	};
}