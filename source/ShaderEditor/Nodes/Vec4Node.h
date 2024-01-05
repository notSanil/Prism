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

	NODE(Float4)
private:
	glm::vec4 m_Value;
};

