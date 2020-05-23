#include "LowRenderer/Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
namespace LowRenderer
{
	Camera::Camera(vec3 position, vec3 front, vec3 up, float yaw, float pitch) : m_movementSpeed(g_speed), m_sensitivity(g_sensitivity), m_zoom(g_zoom)
	{
		m_cameraPos = position;
		m_cameraFront = front;
		m_worldUp = up;
		m_yaw = yaw;
		m_pitch = pitch;
		UpdateVectors();
	}

	Core::Maths::mat4 Camera::CreateOrthographicMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far)
	{
		return { 2 / (right - left),					 0.f,											0.f,					0.f,
					0.f,						 2 / (top - bottom),									0.f,					0.f,
					0.f,								 0.f,										-2 / (far - near),			0.f,
		-(right + left) / (right - left),	-(top + bottom) / (top - bottom),				 -(far + near) / (far - near),		1.f };
	}

	Core::Maths::mat4 Camera::CreateProjectionMatrix(float fov, const float aspectRatio, const float near, const float far)
	{
		fov *= (float)(M_PI / 180);
		float top = tanf(fov * 0.5f) * near;
		float bot = -top;
		float right = top * aspectRatio;
		float left = -right;

		return { (2.f * near) / (right - left),			0.f,						(right + left) / (right - left),			   0.f,
						  0.f,				  (2.f * near) / (top - bot),				(top + bot) / (top - bot),						 0.f,
						  0.f,							0.f,						 -(far + near) / (far - near),					 (-2.f * far * near) / (far - near),
						  0.f,							0.f,						-1.f,											 0.f
		};
	}

	Core::Maths::mat4 Camera::GetViewMatrix()
	{
		return Resources::Transform::LookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
	}

	void Camera::MoveCamera(MovCamera direction, float deltaTime)
	{
		float velocity = m_movementSpeed * deltaTime;
		if (direction == MovCamera::FORWARD)
			m_cameraPos += m_cameraFront * velocity;
		if (direction == MovCamera::BACKWARD)
			m_cameraPos -= m_cameraFront * velocity;
		if (direction == MovCamera::LEFT)
			m_cameraPos += m_cameraRight * velocity;
		if (direction == MovCamera::RIGHT)
			m_cameraPos -= m_cameraRight * velocity;
	}

	void Camera::MouseMoveCamera(float xOffset, float yOffset, bool contrainHeight)
	{
		xOffset *= m_sensitivity;
		yOffset *= m_sensitivity;

		m_yaw += xOffset;
		m_pitch += yOffset;

		if (contrainHeight)
		{
			if (m_pitch > 89.0f)
			{
				m_pitch = 89.0f;
			}
			if (m_pitch < -89.0f)
			{
				m_pitch = -89.0f;
			}
		}

		UpdateVectors();
	}

	void Camera::UpdateVectors()
	{
		// Calculate the new Front vector
		float yaw, pitch;
		yaw = m_yaw * (float)(M_PI / 180);
		pitch = m_pitch * (float)(M_PI / 180);

		vec3 front{ 0.f, 0.f,0.f };
		front.m_x = cosf(yaw) * cosf(pitch);
		front.m_y = sinf(pitch);
		front.m_z = sinf(yaw) * cosf(pitch);

		//re - calculate the right and up
		m_cameraFront = front.Normalize();
		m_cameraRight = m_worldUp.Cross(front).Normalize();
		m_cameraUp = front.Cross(m_cameraRight).Normalize();
	}
}