#include "pch.h"
#include "Vec3Node.h"
#include <imnodes.h>
#include <imgui.h>

Vec3Node::Vec3Node(int id)
	:Node(id)
{
	m_OutputAttribute = { DataType::Float3, &m_Value, "" };
	m_Value = glm::vec3(0.0f);
}

Vec3Node::~Vec3Node()
{}

void Vec3Node::Render()
{}

void Vec3Node::GuiRender()
{
	ImNodes::BeginNode(m_Id);

	ImNodes::BeginNodeTitleBar();
	ImGui::Text(GetName());
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginOutputAttribute(c_OutputStart + 0);
	ImGui::PushItemWidth(200);
	ImGui::DragFloat3("##value", &m_Value.x);
	ImGui::PopItemWidth();
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
}

const OutputAttribute* Vec3Node::GetOutputAttribute(int idx) const
{
	return &m_OutputAttribute;
}

const InputAttribute* Vec3Node::GetInputAttribute(int idx) const
{
	return nullptr;
}

InputAttribute* Vec3Node::GetInputAttribute(int idx)
{
	return nullptr;
}
