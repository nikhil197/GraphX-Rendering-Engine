#pragma once

#include "Entity.h"

namespace GraphX
{
	class CameraController;

	/* Camera Entity */
	/**
		Only Contains the data that represents a Camera in the world.
		For a controlable camera, Use CameraController
	*/
	class Camera
		: public Entity
	{
		friend CameraController;
	public:
		/* Constructor */
		Camera(const GM::Vector3& Position, const GM::Vector3& LookAtPoint, const GM::Vector3& UpAxis); // Matrices will be set by the CameraController

		/* Update the camera */
		virtual void Update(float DeltaTime) override;

		/* Returns whether the camera has been updated or not */
		inline bool IsRenderStateDirty() const { return m_RenderStateDirty; }

		/* Sets the new render state */
		inline void SetRenderStateDirty(bool NewRenderState) { m_RenderStateDirty = NewRenderState; }

		/* Returns a view matrix for the camera */
		inline const GM::Matrix4& GetViewMatrix() const { return m_ViewMatrix; }

		/* Returns the product of projection and view matrix */
		inline const GM::Matrix4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }

		/* Returns the projection matrix for the current camera settings */
		inline const GM::Matrix4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		/* Returns the current controller for the camera */
		inline CameraController* GetCameraController() const { return m_Controller; }

		void SetCameraController(CameraController* Controller) { m_Controller = Controller; }

	private:
		virtual void Enable(class Shader& shader, const std::string& EntityNameInShader = "") const override;

		virtual void Disable() const override;

	private:
		/* Position of the camera */
		GM::Vector3 m_Position;

		/* Pitch, Yaw and Roll of the camera (Euler angles should not be modified directly) */
		GM::Vector3 m_EulerAngles;

		/* Up Axis for the whole world */
		GM::Vector3 m_UpAxis;

		/* Point on which the camera is focussed */
		GM::Vector3 m_LookAtPoint;

		/* Whether the camera settings or orientation has changed */
		bool m_RenderStateDirty = true;

		/* The View matrix for the current camera orientation */
		GM::Matrix4 m_ViewMatrix;

		/* Projection matrix for the current camera settings */
		GM::Matrix4 m_ProjectionMatrix;

		/* Product of the view and the projection matrix */
		GM::Matrix4 m_ProjectionViewMatrix;

		/* Controller of this camera */
		CameraController* m_Controller = nullptr;
	};
}