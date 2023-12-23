#pragma once
#include "Node.h"
#include <glm.hpp>

class Vec3Node : public Node
{
public:
	Vec3Node(int id);
	~Vec3Node() override;

	virtual void Render() override;
	virtual void GuiRender() override;

	virtual const OutputAttribute* GetOutputAttribute(int idx) const override;
	virtual const InputAttribute* GetInputAttribute(int idx) const override;
	virtual InputAttribute* GetInputAttribute(int idx) override;

	NODE(Float3)

private:
	OutputAttribute m_OutputAttribute;

	glm::vec3 m_Value;
};