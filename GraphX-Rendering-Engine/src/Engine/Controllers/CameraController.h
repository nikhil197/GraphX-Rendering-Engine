#pragma once

namespace GraphX
{
	/* Operating mode for the camera */
	enum class ProjectionMode : uint8_t
	{
		Orthographic,
		Perspective
	};

	class Camera;

	class CameraController
	{
	public:
		CameraController(const GM::Vector3& CameraPos, const GM::Vector3& LookAtPoint, const GM::Vector3& UpAxis, float AspectRatio = (4.0f / 3.0f), float Near = 0.1f, float Far = 1000.0f, float FOV = 45.0f);

		//CameraController(const CameraController&) = delete;

		virtual ~CameraController();

		/* Update the camera */
		void Update(float DeltaTime);

		/* Handles events */
		void OnEvent(class Event& e);

		/* Returns the controlled camer */
		inline Ref<Camera> GetCamera() { return m_Camera; }
		
		inline const Camera& GetCamera() const { return *m_Camera; }

		/* Returns the current position of the camera */
		const GM::Vector3& GetCameraPosition() const;

		void SetCameraPosition(const GM::Vector3& NewPosition);

		/* Returns the current orientation of the camera */
		inline const GM::Vector3& GetCameraOrientation() const;

		void SetCameraOrientation(const GM::Vector3& NewOrientation);

		/* Returns the current aspect ratio of the camera */
		inline float GetAspectRatio() const { return m_AspectRatio; }

		/* Sets the new aspect ratio for the camera */
		void SetAspectRatio(float NewRatio);

		/* Returns the current near clip plane of the camera */
		inline float GetNearClipPlane() const { return m_NearClipPlane; }

		/* Sets the new Near plane for the camera */
		void SetNearClipPlane(float NewNear);

		/* Returns the current far clip plane of the camera */
		inline float GetFarClipPlane() const { return m_FarClipPlane; }

		/* Sets the new Far plane for the camera */
		void SetFarClipPlane(float NewFar);

		/* Returns the current project mode for the camera */
		inline ProjectionMode GetProjectionMode() const { return m_CurrentProjectionMode; }

		/* Sets the new projection mode */
		void SetProjectionMode(ProjectionMode NewMode);

		/* Returns the Field of view of the camera */
		inline float GetFieldOfView() const { return m_FieldOfView; }

		/* Sets the new FOV for the camera */
		void SetFieldOfView(float NewFOV);

		/* Returns the width of the current viewport */
		inline float GetOrthoWidth() const { return m_OrthoWidth; }

		/* Sets new width for the viewport */
		void SetOrthoWidth(float NewWidth);

		/* Returns the height of the current viewport */
		inline float GetOrthoHeight() const { return m_OrthoHeight; }

		/* Sets new height for the viewport */
		void SetOrthoHeight(float NewHeight);

	private:
		/* Process the input from the keyboard */
		void ProcessKeyboardInput(float DeltaTime);

		/* Process the input from the mouse */
		void ProcessMouseInput(float DeltaTime);

		/* Updates the projection view matrix if the camera configuration changes */
		void UpdateProjectionViewMatrix();

		/*  Updates the camera on window / viewport resize */ 
		bool OnViewPortResize(class WindowResizedEvent& e);

		/* Updates the camera on mouse scroll */
		bool OnMouseScroll(class MouseScrolledEvent& e);
	
	public:
		/* Camera Translation Speed */
		float TranslationSpeed = 5.5f;

		/* Camera Roll Speed */
		float RollSpeed = 90.0f;

	private:
		/* Camera view axis */
		GM::Vector3 m_ViewAxis;

		/* Right axis in view coordinates */
		GM::Vector3 m_RightAxis;

		/* Up Camera of the camera */
		GM::Vector3 m_UpAxis;

		/* The width of the orthographic view or screen (ignored in perspective mode)  */
		float m_OrthoWidth = 1920.0f;

		/* The height of the orthographic view or screen (ignored in perspective mode)  */
		float m_OrthoHeight = 1080.0f;

		/* The aspect ratio of the camera frame */
		float m_AspectRatio;

		/* The z - coordinate of the near plane behind which nothing will be rendered */
		float m_NearClipPlane;

		/* The z - coordinate of the far plane beyond which nothing will be rendered */
		float m_FarClipPlane;

		/* Horizontal Field of view of the camera (in degrees) in perspective mode (ignored in orthographic mode) */
		float m_FieldOfView;

		/* Current Level of zoom of the camera */
		float m_ZoomLevel = 1.0f;

		/* Whether camera orientation has been changed */
		bool m_ViewChanged = true;

		/* Whether the projection data has been changed */
		bool m_ProjDataChanged = true;

		/* Current camera projection mode */
		ProjectionMode m_CurrentProjectionMode = ProjectionMode::Perspective;

		/* Camera To Control */
		Ref<Camera> m_Camera;
	};
}