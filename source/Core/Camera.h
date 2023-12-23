#pragma once 
#include <glm.hpp>
#include "Events/Event.h"


class Camera
{
public:
	Camera(float aspectRatio);
	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;
	glm::mat4 GetProjectionViewMatrix() const;
	void Update(float deltaTime);
	void OnEvent(const Event& e);
	void ResizeView(uint32_t width, uint32_t height);

	const glm::vec3& GetPosition() const { return m_Position; }

private:
	void RecalculateViewMatrix();
	void RecalculateProjectionMatrix(float aspectRatio);

private:
	float m_FOV = 45.0f;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

	glm::vec3 m_Position;
	glm::vec3 m_CameraTarget;

	float m_CameraDistance = 5.0f;

	float m_Pitch = 0.0f;
	float m_Yaw = -90.0f;

	double m_PreviousX = 0.0;
	double m_PreviousY = 0.0;

	float m_MouseSenstivity = 0.1f;
	float m_ScrollSenstivity = 0.1f;

	static float const c_NearClipPlane;
	static float const c_FarClipPlane;
};

