#include "pch.h"
#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(uint32_t width, uint32_t height, const void* data, TextureType type)
	: m_Type(type)
{
	Create(width, height, data);
}

Texture::Texture(const std::string& path)
	:m_Type(TextureType::Color)
{
	int width, height, channels;
	unsigned char* data = nullptr;
	data = stbi_load(path.c_str(), &width, &height, &channels, 4);

	if (data == NULL)
	{
		LOG_ERROR("Texture Loading failed");
		m_RendererId = 0;
		return;
	}

	Create(width, height, data);

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererId);
}

Texture::Texture(Texture&& other) noexcept
{
	*this = std::move(other);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	std::swap(other.m_RendererId, m_RendererId);
	std::swap(other.m_Type, m_Type);

	return *this;
}

void Texture::Create(uint32_t width, uint32_t height, const void* data)
{
	glGenTextures(1, &m_RendererId);
	glBindTexture(GL_TEXTURE_2D, m_RendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (m_Type == TextureType::Color)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else if (m_Type == TextureType::DepthStencil)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(int textureOffset)
{
	glActiveTexture(GL_TEXTURE0 + textureOffset);
	glBindTexture(GL_TEXTURE_2D, m_RendererId);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
