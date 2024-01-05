#pragma once
#include "Attributes.h"
#include <optional>
enum class NodeType
{
#define DATA_TYPE(X) X,
#include "Rendering/GlslDataTypes-def.h"
#undef DATA_TYPE
	Shader,
	Texture,
};

#define NODE(x) static constexpr const char* GetName() {return #x;} \
virtual constexpr NodeType GetType() const {return NodeType::x;} \
friend class NodeManager;


class Node
{
public:
	Node(int id);
	virtual ~Node() = 0;

	inline int GetId() const { return m_Id; }

	virtual void Render() = 0;
	virtual void GuiRender() = 0;
	const OutputAttribute* GetOutputAttribute(size_t idx) const { return idx < m_OutputAttributes.size() ? &m_OutputAttributes.at(idx) : nullptr; };
	const InputAttribute* GetInputAttribute(size_t idx) const {return idx < m_InputAttributes.size() ? &m_InputAttributes.at(idx) : nullptr;};
	InputAttribute* GetInputAttribute(int idx) { return idx < m_InputAttributes.size() ? &m_InputAttributes.at(idx) : nullptr; };

	NODE(None)

protected:
	int m_Id = 0;
	const int c_InputStart, c_OutputStart, c_StaticStart;
	std::vector<InputAttribute> m_InputAttributes;
	std::vector<OutputAttribute> m_OutputAttributes;
};

