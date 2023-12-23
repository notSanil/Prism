#pragma once
#include "Rendering/Framebuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/AntiAliasedFramebuffer.h"
#include "Core/Camera.h"
#include "Rendering/Light.h"
#include "Events/Event.h"


class Renderspace
{
public:
	Renderspace() = default;
	Renderspace(std::string name);
	Renderspace(std::string name, std::unique_ptr<Framebuffer>&& framebuffer, Shader* shader);
	Renderspace(std::string name, std::unique_ptr<AntiAliasedFramebuffer>&& framebuffer, Shader* shader);

	void Render(std::function<void(Shader*)> renderCommands, bool isPostProcessing, Light& light);
	void RenderGui();
	void OnEvent(const Event& e);

	inline void SetFramebuffer(std::unique_ptr<Framebuffer> framebuffer) { m_Framebuffer = std::move(framebuffer); };
	inline void SetShader(Shader* shader) { m_Shader = shader; };
	inline const Framebuffer& GetFramebuffer() const { return *m_Framebuffer; };
	inline Framebuffer& GetFramebuffer() { return *m_Framebuffer; }

private:
	std::string m_Name;
	std::unique_ptr<Framebuffer> m_Framebuffer = nullptr;
	Shader* m_Shader = nullptr;

	int32_t m_RenderWindowWidth = 0, m_RenderWindowHeight = 1;
	Camera m_Camera{ 0.0f };
};

