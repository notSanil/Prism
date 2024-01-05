#include "pch.h"
#include "Vec3Node.h"
#include <imnodes.h>
#include <imgui.h>

Vec3Node::Vec3Node(int id)
	:Node(id)
{
	m_OutputAttributes.push_back({ DataType::Float3, &m_Value, "" });
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
