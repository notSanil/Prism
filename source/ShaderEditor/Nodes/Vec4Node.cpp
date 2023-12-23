#include "pch.h"
#include "Vec4Node.h"
#include <imgui.h>
#include <imnodes.h>

Vec4Node::Vec4Node(int id)
	:Node(id)
{
	m_OutputAttribute = { DataType::Float4, &m_Value, "" };
	m_Value = glm::vec4(0.0);
}

Vec4Node::~Vec4Node()
{}

void Vec4Node::Render()
{}

void Vec4Node::GuiRender()
{
	ImNodes::BeginNode(m_Id);

	ImNodes::BeginNodeTitleBar();
	ImGui::Text(GetName());
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginOutputAttribute(c_OutputStart + 0);
	ImGui::PushItemWidth(200);
	ImGui::DragFloat4("##value", &m_Value.x);
	ImGui::PopItemWidth();
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();

}
