#include "pch.h"
#include "Renderer.h"
#include <glad/glad.h>
#include "Events/WindowEvents.h"

#ifdef _DEBUG
static void __stdcall glErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	LOG_ERROR(message);
}
#endif // _DEBUG

Shader* Renderer::s_CurrentShader = nullptr;

void Renderer::Init(int windowWidth, int windowHeight)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glErrorMessageCallback, nullptr);
#endif // DEBUG
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(Model& model, const Material& material)
{
	s_CurrentShader->UploadFloatUniform("material.shininess", material.shininess);

	model.GetVao().Bind();
	model.GetShaderStorageBuffer().Bind(s_CurrentShader->GetShaderStorageBindingPoint("transforms"));

	glDrawElements(GL_TRIANGLES, model.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::OnEvent(const Event& e) 
{
	if (e.GetType() == EventType::WindowResizeEvent)
	{
		const WindowResize& resizeEvent = static_cast<const WindowResize&>(e);
		SetViewport(resizeEvent.GetWidth(), resizeEvent.GetHeight());
	}
}

void Renderer::SetWireframeMode(bool isWireframeMode)
{
	if (isWireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::SetViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Renderer::ChangeShader(Shader* shader)
{
	s_CurrentShader = shader;
	s_CurrentShader->Bind();
}

void Renderer::BeginScene(const Light& sceneLight, const Camera& sceneCamera)
{
	s_CurrentShader->UploadVec3Uniform("light.ambient", sceneLight.ambient);
	s_CurrentShader->UploadVec3Uniform("light.diffuse", sceneLight.diffuse);
	s_CurrentShader->UploadVec3Uniform("light.specular", sceneLight.specular);
	s_CurrentShader->UploadVec3Uniform("light.position", sceneLight.position);

	s_CurrentShader->UploadMat4Uniform("viewProj", sceneCamera.GetProjectionViewMatrix());
	s_CurrentShader->UploadVec3Uniform("viewDirection", sceneCamera.GetPosition());
}

void Renderer::EndScene()
{

}

void Renderer::BeginSecondRender()
{
	s_CurrentShader->UploadMat4Uniform("viewProj", glm::mat4(1.0));
}

void Renderer::EndSecondRender()
{
}
