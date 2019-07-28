#pragma once

#include "Entity.h"

namespace engine
{
	/* Operating mode for the camera */
	enum class ProjectionMode : uint8_t
	{
		Orthographic,
		Perspective
	};

	class Camera
		: public Entity
	{
	private:
		/* Up Axis for the whole world */
		gm::Vector3 m_WorldUpAxis;

		/* Camera view axis */
		gm::Vector3 m_ViewAxis;

		/* Right axis in view coordinates */
		gm::Vector3 m_RightAxis;

		/* Up Camera of the camera */
		gm::Vector3 m_UpAxis;

		/* The View matrix for the current camera orientation */
		gm::Matrix4 m_ViewMatrix;

		/* Projection matrix for the current camera settings */
		gm::Matrix4 m_ProjectionMatrix;

		/* Product of the view and the projection matrix */
		gm::Matrix4 m_ProjectionViewMatrix;

		/* The width of the orthographic view or screen (ignored in perspective mode)  */
		float m_OrthoWidth = 1920.0f;

		/* The height of the orthographic view or screen (ignored in perspective mode)  */
		float m_OrthoHeight = 1080.0f;

		/* The aspect ratio of the camera frame */
		float m_AspectRatio;

		/* The z - coordinate of the near plane behind which nothing will be rendered */
		float m_Near;

		/* The z - coordinate of the far plane beyond which nothing will be rendered */
		float m_Far;

		/* Horizontal Field of view of the camera (in degrees) in perspective mode (ignored in orthographic mode) */
		float m_FieldOfView;

		/* Whether the camera settings or orientation has changed */
		bool m_RenderStateDirty = true;

		/* Whether camera orientation has been changed */
		bool m_ViewChanged = false;

		/* Whether the projection data has been changed */
		bool m_ProjDataChanged = false;

		/* Current camera projection mode */
		ProjectionMode m_CurrentProjectionMode = ProjectionMode::Perspective;

	public:
		/* Camera Attributes */
		/* Position of the camera */
		gm::Vector3 CameraPosition;

		/* Pitch, Yaw and Roll of the camera (Euler angles should not be modified directly) */
		gm::Vector3 EulerAngles;

		/* Camera settings for control */
		float CameraSpeed = 5.5f;

	private:
		/* Process the input from the keyboard */
		void ProcessKeyboardInput(float DeltaTime);

		/* Process the input from the mouse */
		void ProcessMouseInput(float DeltaTime);

		/* Calculates the projection matrix based on the current projection mode */
		void CalculateProjectionMatrix();

	public:
		/* Constructor */
		Camera(const gm::Vector3& CameraPos, const gm::Vector3& LookAtPoint, const gm::Vector3& UpAxis, float AspectRatio = (4.0f / 3.0f), float Near = 0.1f, float Far = 1000.0f, float FOV = 45.0f);

		/* Update the camera */
		void Update(float DeltaTime) override;

		/* Sets the new aspect ratio for the camera */
		inline void SetAspectRatio(float NewRatio)
		{ 
			if (m_AspectRatio != NewRatio)
			{
				m_AspectRatio = NewRatio;
				CalculateProjectionMatrix();
				m_RenderStateDirty = true;
			}
		}

		/* Sets the new Near plane for the camera */
		inline void SetNearPlane(float NewNear)
		{ 
			if (m_Near != NewNear)
			{
				m_Near = NewNear;
				CalculateProjectionMatrix();
				m_RenderStateDirty = true;
			}
		}

		/* Sets the new Far plane for the camera */
		inline void SetFarPlane(float NewFar)
		{ 
			if (m_Far != NewFar)
			{
				m_Far = NewFar;
				CalculateProjectionMatrix();
				m_RenderStateDirty = true;
			}
		}

		/* Sets the new projection mode */
		inline void SetNewProjectionMode(ProjectionMode NewMode)
		{
			if (m_CurrentProjectionMode != NewMode)
			{
				m_CurrentProjectionMode = NewMode;
				CalculateProjectionMatrix();
				m_RenderStateDirty = true;
			}
		}

		/* Sets the new FOV for the camera */
		inline void SetNewFOV(float NewFOV)
		{ 
			if (m_FieldOfView != NewFOV && m_CurrentProjectionMode == ProjectionMode::Perspective)
			{
				m_FieldOfView = NewFOV; 
				CalculateProjectionMatrix();
				m_RenderStateDirty = true;
			}
		}

		/* Sets new width for the orthographic view */
		inline void SetOrthoWidth(float NewWidth)
		{
			if (m_OrthoWidth != NewWidth && m_CurrentProjectionMode == ProjectionMode::Orthographic)
			{
				m_OrthoWidth = NewWidth;
				CalculateProjectionMatrix();
				m_RenderStateDirty = true;
			}
		}

		/* Sets new height for the orthographic view */
		inline void SetOrthoHeight(float NewHeight)
		{
			if (m_OrthoHeight != NewHeight && m_CurrentProjectionMode == ProjectionMode::Orthographic)
			{
				m_OrthoHeight = NewHeight;
				CalculateProjectionMatrix();
				m_RenderStateDirty = true;
			}
		}

		/* Returns whether the camera has been updated or not */
		inline bool IsRenderStateDirty() const { return m_RenderStateDirty; }

		/* Sets the new render state */
		inline void SetRenderStateDirty(bool NewRenderState) { m_RenderStateDirty = NewRenderState; }

		/* Returns the current project mode for the camera */
		inline ProjectionMode GetProjectionMode() const { return m_CurrentProjectionMode; }

		/* Returns the Field of view of the camera */
		inline float GetFieldOfView() const 
		{
			if (m_CurrentProjectionMode == ProjectionMode::Perspective)
				return m_FieldOfView;
			else
				return 0.0f;
		}

		/* Returns a view matrix for the camera */
		inline const gm::Matrix4& GetViewMatrix() const { return m_ViewMatrix; }

		/* Returns the product of projection and view matrix */
		inline const gm::Matrix4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }

		/* Returns the projection matrix for the current camera settings */
		inline const gm::Matrix4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		void Enable(class Shader& shader, const std::string& Name) const override;

		void Disable() const override;
	};
}