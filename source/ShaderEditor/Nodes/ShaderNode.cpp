#include "pch.h"
#include "ShaderNode.h"
#include <imnodes.h>

ShaderNode::ShaderNode(int id)
	:Node(id)
{
	m_InputAttributes.push_back({ DataType::File, nullptr, "Vertex Shader" });
	m_InputAttributes.push_back({ DataType::File, nullptr, "Fragment Shader" });
}

ShaderNode::~ShaderNode()
{

}

void ShaderNode::GuiRender()
{
	ImNodes::BeginNode(m_Id);
	
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(GetName());
	ImNodes::EndNodeTitleBar();

	for (int i = 0; i < m_InputAttributes.size(); ++i)
	{
		ImNodes::BeginInputAttribute(c_InputStart + i);
		ImGui::Text("%s (%s)", m_InputAttributes.at(i).Name.c_str(), DataTypeToString[static_cast<int>(m_InputAttributes.at(i).Type)].c_str());
		ImNodes::EndInputAttribute();
	}

	ImNodes::EndNode();
}

void ShaderNode::Render()
{
	if (m_InputAttributes[0].Value && m_InputAttributes[1].Value && !m_Shader)
	{
		m_Shader = std::make_unique<Shader>(*static_cast<const std::string*>(m_InputAttributes[0].Value), *static_cast<const std::string*>(m_InputAttributes[1].Value));

		for (auto& [name, uniformInfo] : m_Shader->GetUniformInfo())
		{
			m_InputAttributes.push_back({ uniformInfo.Type, nullptr, name });
		}
	}
}