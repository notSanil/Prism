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
virtual constexpr NodeType GetType() const {return NodeType::x;}


class Node
{
public:
	Node(int id);
	virtual ~Node() = 0;

	inline int GetId() const { return m_Id; }

	virtual void Render() = 0;
	virtual void GuiRender() = 0;
	virtual const OutputAttribute* GetOutputAttribute(int idx) const = 0;
	virtual const InputAttribute* GetInputAttribute(int idx) const = 0;
	virtual InputAttribute* GetInputAttribute(int idx) = 0;

	NODE(None)

protected:
	int m_Id = 0;
	const int c_InputStart, c_OutputStart, c_StaticStart;
};

