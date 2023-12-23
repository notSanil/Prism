#pragma once
#include <vector>
#include "Buffer.h"

enum class AttributeType
{
	None,
	FLOAT, VEC2, VEC3, VEC4,
	INT, IVEC2, IVEC3, IVEC4,
};

enum class AttributeClass
{
	None,
	Float,
	Int
};


struct Attribute
{
	bool normalised = false;
	AttributeType type;
};

struct AttributeSpecification
{
	AttributeSpecification(std::initializer_list<Attribute> attributes)
		:attribs(attributes)
	{
		offsets = std::vector<size_t>(attributes.size(), -1);

		size_t currentOffset = 0;
		for (int i = 0; i < attribs.size(); ++i)
		{
			offsets[i] = currentOffset;
			currentOffset += SizeOf(attribs[i].type);
		}

		stride = currentOffset;
	}

	std::vector<Attribute> attribs;
	std::vector<size_t> offsets;
	size_t stride;	

private:
	size_t SizeOf(AttributeType attribute)
	{
		switch (attribute)
		{
		case AttributeType::None:
			return 0;
		case AttributeType::FLOAT:
			return sizeof(float);
		case AttributeType::VEC2:
			return 2 * sizeof(float);
		case AttributeType::VEC3:
			return 3 * sizeof(float);
		case AttributeType::VEC4:
			return 4 * sizeof(float);
		case AttributeType::INT:
			return sizeof(int);
		case AttributeType::IVEC2:
			return 2 * sizeof(int);
		case AttributeType::IVEC3:
			return 3 * sizeof(int);
		case AttributeType::IVEC4:
			return 4 * sizeof(int);
		default:
			return 0;
		}
	}
};


class VertexAttributeObject
{
public:
	VertexAttributeObject() = default;
	VertexAttributeObject(Buffer& vertexBuffer, Buffer& indexBuffer, 
		const AttributeSpecification& specification);
	VertexAttributeObject(const VertexAttributeObject& other) = delete;
	VertexAttributeObject(VertexAttributeObject&& other) noexcept;
	~VertexAttributeObject();
	VertexAttributeObject& operator=(const VertexAttributeObject& other) = delete;
	VertexAttributeObject& operator=(VertexAttributeObject&& other) noexcept;

	void Bind();
	void Unbind();
	inline uint32_t GetNumberOfElements() { return m_ElementCount; }
	inline uint32_t GetVertexCount() { return m_VertexCount; }

private:
	uint32_t m_RendererId = 0;
	uint32_t m_ElementCount = 0;
	uint32_t m_VertexCount = 0;
};

