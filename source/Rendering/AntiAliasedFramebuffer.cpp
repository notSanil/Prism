#include "pch.h"
#include "AntiAliasedFramebuffer.h"
#include <glad/glad.h>

const uint32_t AntiAliasedFramebuffer::samples = 4;

AntiAliasedFramebuffer::AntiAliasedFramebuffer(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;
	glGenFramebuffers(1, &m_RendererId);
	Bind();
	RemakeBuffers();
	Unbind();

	m_aliasedFramebuffer = Framebuffer(width, height);
}

AntiAliasedFramebuffer::AntiAliasedFramebuffer(AntiAliasedFramebuffer&& other) noexcept
{
	*this = std::move(other);
}

AntiAliasedFramebuffer& AntiAliasedFramebuffer::operator=(AntiAliasedFramebuffer&& other) noexcept
{
	Framebuffer::operator=(std::move(other));
	std::swap(other.m_aliasedFramebuffer, m_aliasedFramebuffer);
	return *this;
}

void AntiAliasedFramebuffer::BlitToFramebuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reinterpret_cast<uint64_t>(m_aliasedFramebuffer.GetRendererId()));
	glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void AntiAliasedFramebuffer::Resize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	glDeleteTextures(1, &m_ColorTexture);
	glDeleteTextures(1, &m_DepthTexture);

	Bind();
	RemakeBuffers();
	Unbind();

	m_aliasedFramebuffer.Resize(width, height);
}

void AntiAliasedFramebuffer::RemakeBuffers()
{
	glGenTextures(1, &m_ColorTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorTexture);

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, m_Width, m_Height, GL_TRUE);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorTexture, 0);

	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthTexture);

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH24_STENCIL8, m_Width, m_Height, GL_TRUE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthTexture, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Error occured while creating framebuffer!");
	}
}