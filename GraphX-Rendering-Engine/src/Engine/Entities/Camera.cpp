#include "pch.h"
#include "Camera.h"

#include "Timer/Clock.h"

/* Input */
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace GraphX
{
	Camera::Camera(const GM::Vector3& Position, const GM::Vector3& LookAtPoint, const GM::Vector3& UpAxis)
		: m_Position(Position), m_Rotation(0), m_UpAxis(UpAxis), m_LookAtPoint(LookAtPoint), m_ProjectionMatrix(GM::Matrix4()), m_ViewMatrix(GM::Matrix4()), m_ProjectionViewMatrix(GM::Matrix4())
	{

	}

	void Camera::Update(float DeltaTime)
	{

	}

	void Camera::Enable(class Shader& shader, const std::string& Name) const
	{
	}

	void Camera::Disable() const
	{
	}
}