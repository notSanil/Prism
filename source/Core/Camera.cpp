#include "pch.h"
#include "Camera.h"
#include <gtc/matrix_transform.hpp>
#include "Core/Input.h"
#include "Events/WindowEvents.h"
#include <GLFW/glfw3.h>
//TODO: add own key codes

float const Camera::c_FarClipPlane = 100.0f;
float const Camera::c_NearClipPlane = 0.1f;

Camera::Camera(float aspectRatio)
{
	m_Position = glm::vec3(0.0f, 2.0f, 6.0f);

	m_CameraDistance = glm::length(m_Position);
	m_CameraTarget = glm::vec3();
	
	RecalculateProjectionMatrix(aspectRatio);
	RecalculateViewMatrix();
}

void Camera::Update(float deltaTime)
{

}

void Camera::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::MouseMovedEvent)
	{
		const MouseMoved& mouseMoved = static_cast<const MouseMoved&>(e);

		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			double xOffset = mouseMoved.GetMouseX() - m_PreviousX;
			double yOffset = m_PreviousY - mouseMoved.GetMouseY();

			m_Pitch += yOffset * m_MouseSenstivity;
			m_Yaw += xOffset * m_MouseSenstivity;

			RecalculateViewMatrix();
		}

		m_PreviousX = mouseMoved.GetMouseX();
		m_PreviousY = mouseMoved.GetMouseY();
	}
	else if (e.GetType() == EventType::MouseScrollEvent)
	{
		const MouseScroll& mouseScroll = static_cast<const MouseScroll&>(e);

		m_CameraDistance -= mouseScroll.GetScrolly() * m_ScrollSenstivity * m_CameraDistance;
		RecalculateViewMatrix();
	}
	else if (e.GetType() == EventType::WindowResizeEvent)
	{
		const WindowResize& windowResize = static_cast<const WindowResize&>(e);

		auto windowWidth = windowResize.GetWidth();
		auto windowHeight = windowResize.GetHeight();

		if (windowHeight == 0)
			return;

		float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

		RecalculateProjectionMatrix(aspectRatio);
	}
}

void Camera::RecalculateViewMatrix()
{
	m_Pitch = fmod(m_Pitch, 360.0f);
	m_Yaw = fmod(m_Yaw, 360.0f);

	glm::vec3 focusVector = m_CameraTarget - m_Position;

	focusVector.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	focusVector.y = sin(glm::radians(m_Pitch));
	focusVector.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Position = m_CameraTarget - focusVector;

	m_Position *= m_CameraDistance;

	float y = 1.0f;

	if ((m_Pitch > 90.0f && m_Pitch < 270.0f) || (m_Pitch < -90.0f && m_Pitch > -270.0f))
		y *= -1.0f;

	m_ViewMatrix = glm::lookAt(m_Position, m_CameraTarget, glm::vec3(0.0f, y, 0.0f));
}

void Camera::RecalculateProjectionMatrix(float aspectRatio)
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), aspectRatio, c_NearClipPlane, c_FarClipPlane);
}

void Camera::ResizeView(uint32_t width, uint32_t height)
{
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	RecalculateProjectionMatrix(aspectRatio);
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

glm::mat4 Camera::GetProjectionViewMatrix() const
{
	return m_ProjectionMatrix * m_ViewMatrix;
}