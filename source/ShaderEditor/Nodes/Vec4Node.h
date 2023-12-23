#pragma once
#include "Node.h"
#include <glm.hpp>
class Vec4Node :
    public Node
{
public:
	Vec4Node(int id);
	~Vec4Node();

	virtual void Render() override;
	virtual void GuiRender() override;
	virtual const OutputAttribute* GetOutputAttribute(int idx) const override { return &m_OutputAttribute; };
	virtual const InputAttribute* GetInputAttribute(int idx) const override { return nullptr; };
	virtual InputAttribute* GetInputAttribute(int idx) override { return nullptr; };

	NODE(Float4)
private:
	OutputAttribute m_OutputAttribute;
	glm::vec4 m_Value;
};

