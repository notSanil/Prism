#pragma once
#include "Rendering/Texture.h"


class Framebuffer
{
public:
	Framebuffer() = default;
	Framebuffer(uint32_t width, uint32_t height);
	~Framebuffer();
	Framebuffer(const Framebuffer& other) = delete;
	Framebuffer(Framebuffer&& other) noexcept;
	Framebuffer& operator=(const Framebuffer& other) = delete;
	Framebuffer& operator=(Framebuffer&& other) noexcept;

	void Bind();
	void Unbind();

	virtual Texture& GetColorTexture() { return m_Color; }
	virtual Texture& GetDepthStencilTexture() { return m_DepthStencil; }
	virtual const Texture& GetColorTexture() const { return m_Color; }
	virtual const Texture& GetDepthStencilTexture() const { return m_DepthStencil; }
	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }

	virtual void Resize(uint32_t width, uint32_t height);
	inline void* const GetRendererId() const { return reinterpret_cast<void*>(static_cast<int64_t>(m_RendererId)); };
	virtual void BlitToFramebuffer() {};

protected:
	virtual void RemakeBuffers();

protected:
	uint32_t m_RendererId = 0;
	uint32_t m_Width = 0, m_Height = 0;

	Texture m_Color, m_DepthStencil;
};

