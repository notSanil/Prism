#include "pch.h"
#include "Renderspace.h"
#include "Rendering/Renderer.h"
#include <imgui.h>

Renderspace::Renderspace(std::string name)
	:m_Name(name), m_Camera((float)m_RenderWindowWidth / (float)m_RenderWindowHeight)
{
}

Renderspace::Renderspace(std::string name, std::unique_ptr<Framebuffer>&& framebuffer, Shader* shader)
	:m_Name(name), m_Framebuffer(std::move(framebuffer)), m_Shader(shader)
{
	m_RenderWindowWidth = m_Framebuffer->GetWidth();
	m_RenderWindowHeight = m_Framebuffer->GetHeight();
	m_Camera = Camera((float)m_RenderWindowWidth / (float)m_RenderWindowHeight);
}

Renderspace::Renderspace(std::string name, std::unique_ptr<AntiAliasedFramebuffer>&& framebuffer, Shader* shader)
	: m_Name(name), m_Framebuffer(std::move(framebuffer)), m_Shader(shader)
{
	m_RenderWindowWidth = m_Framebuffer->GetWidth();
	m_RenderWindowHeight = m_Framebuffer->GetHeight();

	m_Camera = Camera((float)m_RenderWindowWidth / (float)m_RenderWindowHeight);
}

void Renderspace::Render(std::function<void(Shader*)> renderCommands, bool isPostProcessing, Light& light)
{ //TODO: These extra variables are hacks, remove ASAP
	bool isRenderSpaceResized = (m_RenderWindowWidth != m_Framebuffer->GetWidth() || m_RenderWindowHeight != m_Framebuffer->GetHeight()) &&
		(m_RenderWindowHeight > 0 && m_RenderWindowWidth > 0);
	if (isRenderSpaceResized)
	{
		m_Framebuffer->Resize(m_RenderWindowWidth, m_RenderWindowHeight);
		m_Camera.ResizeView(m_RenderWindowWidth, m_RenderWindowHeight);
	}

	Renderer::SetViewport(m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight());
	m_Framebuffer->Bind();
	Renderer::ClearScreen();
	Renderer::ChangeShader(m_Shader);

	if (isPostProcessing)
		Renderer::BeginSecondRender();
	else
		Renderer::BeginScene(light, m_Camera);

	renderCommands(m_Shader);

	if (isPostProcessing)
		Renderer::EndSecondRender();
	else
		Renderer::EndScene();

	m_Framebuffer->Unbind();
	m_Framebuffer->BlitToFramebuffer();

}

void Renderspace::RenderGui()
{
	ImGui::Begin(m_Name.c_str(), nullptr);
	ImGui::Image(m_Framebuffer->GetColorTexture().GetRendererID(), { (float)m_RenderWindowWidth, (float)m_RenderWindowHeight }, { 0, 1 }, { 1, 0 });
	ImVec2 maxRegion = ImGui::GetWindowContentRegionMax();
	ImVec2 minRegion = ImGui::GetWindowContentRegionMin();
	m_RenderWindowWidth = static_cast<int32_t>(maxRegion.x - minRegion.x);
	m_RenderWindowHeight = static_cast<int32_t>(maxRegion.y - minRegion.y);
	ImGui::End();
}

void Renderspace::OnEvent(const Event& e)
{
	m_Camera.OnEvent(e);
}
