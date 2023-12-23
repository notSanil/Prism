#include "pch.h"
#include "Framebuffer.h"
#include <glad/glad.h>

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
	:m_Width(width), m_Height(height)
{
	glGenFramebuffers(1, &m_RendererId);
	Bind();
	RemakeBuffers();
	Unbind();
}

void Framebuffer::RemakeBuffers()
{
	m_Color = Texture(m_Width, m_Height, nullptr, TextureType::Color);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reinterpret_cast<uint64_t>(m_Color.GetRendererID()), 0);
	
	m_DepthStencil = Texture(m_Width, m_Height, nullptr, TextureType::DepthStencil);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, reinterpret_cast<uint64_t>(m_DepthStencil.GetRendererID()), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Error occured while creating framebuffer!");
	}
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	Bind();
	RemakeBuffers();
	Unbind();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_RendererId);
	m_RendererId = 0;
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
{
	*this = std::move(other);
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
	std::swap(other.m_RendererId, m_RendererId);
	std::swap(other.m_Width, m_Width);
	std::swap(other.m_Height, m_Height);
	std::swap(other.m_Color, m_Color);
	std::swap(other.m_DepthStencil, m_DepthStencil);

	return *this;
}
