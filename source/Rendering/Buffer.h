#pragma once
#include "Core/Mesh.h"

enum class BufferType
{
	None,
	Vertex,
	Index,
	ShaderStorage
};

class Buffer
{
public:
	Buffer() = default;
	Buffer(const void* data, uint32_t size, BufferType type);
	Buffer(const Mesh& mesh, BufferType type);
	Buffer(const Buffer& other) = delete;
	Buffer(Buffer&& other) noexcept;
	Buffer& operator=(const Buffer& other) = delete;
	Buffer& operator=(Buffer&& other) noexcept;
	~Buffer();

	void Bind(int32_t index = -1);
	void Unbind();
	uint32_t GetNumberOfElements();

private:
	BufferType m_BufferType = BufferType::None;
	uint32_t m_RendererId = 0;

	uint32_t m_ElementCount = 0;
};

