#include "pch.h"
#include "VertexAttributeObject.h"
#include <glad/glad.h>


GLenum VertexSpecToGLSpec(AttributeType attribute)
{
	switch (attribute)
	{
	case AttributeType::None:
		return GL_NONE;
	case AttributeType::FLOAT:
		return GL_FLOAT;
	case AttributeType::VEC2:
		return GL_FLOAT;
	case AttributeType::VEC3:
		return GL_FLOAT;
	case AttributeType::VEC4:
		return GL_FLOAT;
	case AttributeType::INT:
		return GL_INT;
	case AttributeType::IVEC2:
		return GL_INT;
	case AttributeType::IVEC3:
		return GL_INT;
	case AttributeType::IVEC4:
		return GL_INT;
	default:
		return GL_NONE;
	}
}

int NumberOfElements(AttributeType attribute)
{
	switch (attribute)
	{
	case AttributeType::None:
		return 0;
	case AttributeType::FLOAT:
		return 1;
	case AttributeType::VEC2:
		return 2;
	case AttributeType::VEC3:
		return 3;
	case AttributeType::VEC4:
		return 4;
	case AttributeType::INT:
		return 1;
	case AttributeType::IVEC2:
		return 2;
	case AttributeType::IVEC3:
		return 3;
	case AttributeType::IVEC4:
		return 4;
	default:
		return 0;
	}
}

AttributeClass VertexSpecToAttrClass(AttributeType attribute)
{
	switch (attribute)
	{
	case AttributeType::None:
		return AttributeClass::None;
	case AttributeType::FLOAT:
	case AttributeType::VEC2:
	case AttributeType::VEC3:
	case AttributeType::VEC4:
		return AttributeClass::Float;
	case AttributeType::INT:
	case AttributeType::IVEC2:
	case AttributeType::IVEC3:
	case AttributeType::IVEC4:
		return AttributeClass::Int;
	default:
		return AttributeClass::None;
	}
}

VertexAttributeObject::VertexAttributeObject(Buffer& vertexBuffer,
	Buffer& indexBuffer, const AttributeSpecification& specification)
{
	glGenVertexArrays(1, &m_RendererId);
	glBindVertexArray(m_RendererId);
	vertexBuffer.Bind();
	for (int i = 0; i < specification.attribs.size(); ++i)
	{
		if (VertexSpecToAttrClass(specification.attribs[i].type) == AttributeClass::Int)
		{
			glVertexAttribIPointer(i, NumberOfElements(specification.attribs[i].type),
				VertexSpecToGLSpec(specification.attribs[i].type), static_cast<int>(specification.stride),
				(void*)specification.offsets[i]
			);
		}
		else if (VertexSpecToAttrClass(specification.attribs[i].type) == AttributeClass::Float)
		{
			glVertexAttribPointer(i, NumberOfElements(specification.attribs[i].type),
				VertexSpecToGLSpec(specification.attribs[i].type),
				specification.attribs[i].normalised ? GL_TRUE : GL_FALSE, static_cast<int>(specification.stride),
				(void*)specification.offsets[i]
			);
		}
		glEnableVertexAttribArray(i);
	}
	indexBuffer.Bind();

	glBindVertexArray(0);
	vertexBuffer.Unbind();
	indexBuffer.Unbind();

	m_ElementCount = indexBuffer.GetNumberOfElements();
	m_VertexCount = vertexBuffer.GetNumberOfElements() * sizeof(float) / specification.stride;
}

VertexAttributeObject::VertexAttributeObject(VertexAttributeObject&& other) noexcept
{
	m_RendererId = other.m_RendererId;
	m_ElementCount = other.m_ElementCount;
	m_VertexCount = other.m_VertexCount;

	other.m_RendererId = 0;
	other.m_ElementCount = 0;
	other.m_VertexCount = 0;
}

VertexAttributeObject::~VertexAttributeObject()
{
	glDeleteVertexArrays(1, &m_RendererId);
}

VertexAttributeObject& VertexAttributeObject::operator=(VertexAttributeObject&& other) noexcept
{
	std::swap(m_RendererId, other.m_RendererId);
	std::swap(m_ElementCount, other.m_ElementCount);
	std::swap(m_VertexCount, other.m_VertexCount);

	return *this;
}

void VertexAttributeObject::Bind()
{
	glBindVertexArray(m_RendererId);
}

void VertexAttributeObject::Unbind()
{
	glBindVertexArray(0);
}
