#pragma once
#include <string>

enum class TextureType
{
	None = 0,
	Color,
	DepthStencil
};

class Texture
{
public:
	Texture() = default;
	Texture(uint32_t width, uint32_t height, const void* data, TextureType type=TextureType::Color);
	Texture(const std::string& path);
	~Texture();
	Texture(const Texture& other) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(const Texture& other) = delete;
	Texture& operator=(Texture&& other) noexcept;

	void* GetRendererID() const { return (void*)(uint64_t)m_RendererId; }
	void Bind(int textureOffset=0);
	void Unbind();

private:
	void Create(uint32_t width, uint32_t height, const void* data);

private:
	uint32_t m_RendererId = 0;
	TextureType m_Type = TextureType::None;
};

