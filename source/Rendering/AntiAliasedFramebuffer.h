#pragma once
#include "Framebuffer.h"


class AntiAliasedFramebuffer : public Framebuffer
{
public:
	AntiAliasedFramebuffer() = default;
	AntiAliasedFramebuffer(uint32_t width, uint32_t height);
	~AntiAliasedFramebuffer() = default;
	AntiAliasedFramebuffer(const AntiAliasedFramebuffer & other) = delete;
	AntiAliasedFramebuffer(AntiAliasedFramebuffer&& other) noexcept;
	AntiAliasedFramebuffer& operator=(const AntiAliasedFramebuffer & other) = delete;
	AntiAliasedFramebuffer& operator=(AntiAliasedFramebuffer&& other) noexcept;

	void BlitToFramebuffer() override;
	void Resize(uint32_t width, uint32_t height) override;

	Texture& GetColorTexture() override { return m_aliasedFramebuffer.GetColorTexture(); }
	Texture& GetDepthStencilTexture() override { return m_aliasedFramebuffer.GetDepthStencilTexture(); }
	const Texture& GetColorTexture() const override { return m_aliasedFramebuffer.GetColorTexture(); }
	const Texture& GetDepthStencilTexture() const override { return m_aliasedFramebuffer.GetDepthStencilTexture(); }
private:
	void RemakeBuffers() override;

private:
	static const uint32_t samples;
	uint32_t m_ColorTexture, m_DepthTexture;
	Framebuffer m_aliasedFramebuffer;
};

