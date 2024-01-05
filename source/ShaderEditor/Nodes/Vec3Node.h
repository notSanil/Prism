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

	NODE(Float3)

private:
	glm::vec3 m_Value;
};