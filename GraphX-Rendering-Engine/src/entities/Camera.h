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

		/* Whether the camera has been updated and the matrices need to be set again */
		bool m_RenderStateDirty;

		/* Camera settings for control */
		float m_CameraSpeed;

		/* Camera view axis */
		gm::Vector3 m_ViewAxis;

		/* Right axis in view coordinates */
		gm::Vector3 m_RightAxis;

		/* Up Axis in view coordinates */
		gm::Vector3 m_UpAxis;

	public:
		/* Camera Attributes */
		/* Position of the camera */
		gm::Vector3 CameraPosition;

		/* Pitch, Yaw and Roll of the camera (Euler angles should not be modified directly) */
		gm::Vector3 EulerAngles;

		/* Field of view of the camera (in degrees) */
		float FieldOfView;

	private:
		/* Updates the camera axis */
		void UpdateCameraAxes();

		/* Process the input from the keyboard */
		void ProcessKeyboardInput(float DeltaTime);

		/* Process the input from the mouse */
		void ProcessMouseInput(float DeltaTime);

	public:
		/* Constructor */
		Camera(const gm::Vector3& CameraPos, const gm::Vector3& LookAtPoint, const gm::Vector3& UpAxis, float AspectRatio = (4.0f / 3.0f), float Near = 0.1f, float Far = 1000.0f, float FOV = 45.0f);

		/* Update the camera */
		void Update(float DeltaTime);

		/* Sets the new aspect ratio for the camera */
		inline void SetAspectRatio(float Ratio) { m_AspectRatio = Ratio; }

		/* Sets the new Near plane for the camera */
		inline void SetNearPlane(float Near) { m_Near = Near; }

		/* Sets the new Far plane for the camera */
		inline void SetFarPlane(float Far) { m_Far = Far; }

		/* Returns whether the camera has been updated or not */
		inline bool IsRenderStateDirty() const { return m_RenderStateDirty; }

		inline void SetRenderState(bool bNewRenderState) { m_RenderStateDirty = bNewRenderState; }

		/* Returns a view matrix for the camera */
		inline gm::Matrix4 GetViewMatrix() const { return gm::View::LookAt(CameraPosition, m_CameraLookAtPoint, m_WorldUpAxis); }

		/* Returns the projection matrix for the current camera settings */
		inline gm::Matrix4 GetPerspectiveProjectionMatrix() const { return gm::Projection::Perspective(FieldOfView, m_AspectRatio, m_Near, m_Far); }

		/* Returns the orthographic matrix for the current settings and the provided width and height */
		inline gm::Matrix4 GetOrthographiProjectionMatrix(float width, float height) const { return gm::Projection::Ortho(0, width, 0, height, m_Near, m_Far); }
	};
}