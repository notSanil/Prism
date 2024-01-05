#include "pch.h"
#include "TextureNode.h"
#include <imnodes.h>

TextureNode::TextureNode(int id)
	:Node(id)
{
	m_OutputAttributes.push_back({ DataType::Sampler2D, &m_Texture, "Color" });
	m_InputAttributes.push_back({ DataType::File, nullptr, "Texture" });
}

TextureNode::~TextureNode()
{}

void TextureNode::GuiRender()
{
	ImNodes::BeginNode(m_Id);
	
	ImNodes::BeginNodeTitleBar();
	ImGui::Text(GetName());
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(c_InputStart + 0);
	ImGui::Text("%s (%s)", m_InputAttributes[0].Name.c_str(), DataTypeToString[static_cast<int>(m_InputAttributes[0].Type)].c_str());
	ImNodes::EndInputAttribute();

	ImNodes::BeginOutputAttribute(c_OutputStart + 0);
	if (m_Texture)
		ImGui::Image(m_Texture->GetRendererID(), { 100, 100 });
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
}

void TextureNode::Render()
{
	if (m_InputAttributes[0].Value && !m_Texture)
	{
		const std::string& path = *static_cast<const std::string*>(m_InputAttributes[0].Value);

		m_Texture = std::make_unique<Texture>(path);
	}
}
