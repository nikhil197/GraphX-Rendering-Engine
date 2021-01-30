#pragma once

#include "Entity.h"

namespace GraphX
{
	class CameraController;

	/* Camera Entity */
	/**
		Only Contains the data that represents a Camera in the world.
		For a controllable camera, Use CameraController
	*/
	class Camera
		: public Entity
	{
		friend CameraController;
	public:
		/* Constructor */
		Camera(const GM::Vector3& Position, const GM::Vector3& LookAtPoint); // Matrices will be set by the CameraController

		/* Update the camera */
		virtual void Update(float DeltaTime) override;

		/* Returns whether the camera has been updated or not */
		inline bool IsRenderStateDirty() const { return m_RenderStateDirty; }

		/* Sets the new render state */
		inline void SetRenderStateDirty(bool NewRenderState) { m_RenderStateDirty = NewRenderState; }

		/* Returns the position of the camera */
		inline const GM::Vector3& GetPosition() const { return m_Position; }

		/* Returns the rotation of the camera */
		inline const GM::Rotator& GetRotation() const { return m_Rotation; }

		/* Returns a view matrix for the camera */
		inline const GM::Matrix4& GetViewMatrix() const { return m_ViewMatrix; }

		/* Returns a rotation view matrix of the camera */
		inline const GM::Matrix4& GetRotationViewMatrix() const { return m_RotationViewMatrix; }

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

		/** Note: Rotation should not be modified directly */
		/* Orientation of the camera */
		GM::Rotator m_Rotation;

		/* Point on which the camera is focused */
		GM::Vector3 m_LookAtPoint;

		/* The View matrix for the current camera orientation */
		GM::Matrix4 m_ViewMatrix;

		/* The View matrix containing only the camera rotation (to be used by sky boxes) */
		GM::Matrix4 m_RotationViewMatrix;

		/* Projection matrix for the current camera settings */
		GM::Matrix4 m_ProjectionMatrix;

		/* Product of the view and the projection matrix */
		GM::Matrix4 m_ProjectionViewMatrix;

		/* Controller of this camera */
		CameraController* m_Controller = nullptr;	// This is only a weak reference, no need to make this a ref
		
		/* Whether the camera settings or orientation has changed */
		bool m_RenderStateDirty = true;
	};
}