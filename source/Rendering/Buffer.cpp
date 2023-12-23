#include "pch.h"
#include "Buffer.h"
#include <glad/glad.h>

static GLenum BufferTypeToGLType(BufferType type)
{
	switch (type)
	{
	case BufferType::None:
		return GL_NONE;
	case BufferType::Vertex:
		return GL_ARRAY_BUFFER;
	case BufferType::Index:
		return GL_ELEMENT_ARRAY_BUFFER;
	case BufferType::ShaderStorage:
		return GL_SHADER_STORAGE_BUFFER;
	default:
		return GL_NONE;
	}
}

Buffer::Buffer(const void* data, uint32_t size, BufferType type)
	:m_BufferType(type)
{
	glGenBuffers(1, &m_RendererId);
	GLenum glType = BufferTypeToGLType(m_BufferType);
	glBindBuffer(glType, m_RendererId);
	glBufferData(glType, size, data, GL_STATIC_DRAW);
	glBindBuffer(glType, 0);

	if (type == BufferType::Index)
	{
		m_ElementCount = size / (uint32_t)sizeof(uint32_t);
	}
	else if (type == BufferType::Vertex)
	{
		m_ElementCount = size / (uint32_t)sizeof(float);
	}
	else if (type == BufferType::ShaderStorage)
	{
		m_ElementCount = size;
	}
}

Buffer::Buffer(const Mesh& mesh, BufferType type)
	:m_BufferType(type)
{
	glGenBuffers(1, &m_RendererId);
	GLenum glType = BufferTypeToGLType(m_BufferType);
	glBindBuffer(glType, m_RendererId);
	if (m_BufferType == BufferType::Vertex)
	{
		glBufferData(glType, mesh.GetVertexDataSize(), mesh.GetVertexData(), GL_STATIC_DRAW);
		m_ElementCount = mesh.GetVertexDataSize() / sizeof(float);
	}
	else if (m_BufferType == BufferType::Index)
	{
		glBufferData(glType, mesh.GetIndexDataSize(), mesh.GetIndexData(), GL_STATIC_DRAW);
		m_ElementCount = mesh.GetIndexCount();
	}
	else if (m_BufferType == BufferType::ShaderStorage)
	{
		LOG_ERROR("Shader storage buffer cannot be made from a mesh!");
	}
	glBindBuffer(glType, 0);
}

Buffer::Buffer(Buffer&& other) noexcept
{
	*this = std::move(other);
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
	std::swap(m_BufferType, other.m_BufferType);
	std::swap(m_RendererId, other.m_RendererId);
	std::swap(m_ElementCount, other.m_ElementCount);

	return *this;
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_RendererId);
	m_RendererId = 0;
}

void Buffer::Bind(int32_t index)
{
	if (m_BufferType == BufferType::ShaderStorage)
		glBindBufferBase(BufferTypeToGLType(m_BufferType), index, m_RendererId);
	else
		glBindBuffer(BufferTypeToGLType(m_BufferType), m_RendererId);
}

void Buffer::Unbind()
{
	glBindBuffer(BufferTypeToGLType(m_BufferType), 0);
}

uint32_t Buffer::GetNumberOfElements()
{
	return m_ElementCount;
}