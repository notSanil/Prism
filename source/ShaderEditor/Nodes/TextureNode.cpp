#include "pch.h"
#include "TextureNode.h"
#include <imnodes.h>

TextureNode::TextureNode(int id)
	:Node(id)
{
	m_OutputAttribute = { DataType::Sampler2D, &m_Texture, "" };
	m_InputAttribute = { DataType::File, nullptr, "Texture"};
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
	ImGui::Text("%s (%s)", m_InputAttribute.Name.c_str(), DataTypeToString[static_cast<int>(m_InputAttribute.Type)].c_str());
	ImNodes::EndInputAttribute();

	ImNodes::BeginOutputAttribute(c_OutputStart + 0);
	if (m_Texture)
		ImGui::Image(m_Texture->GetRendererID(), { 100, 100 });
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
}

void TextureNode::Render()
{
	if (m_InputAttribute.Value && !m_Texture)
	{
		const std::string& path = *static_cast<const std::string*>(m_InputAttribute.Value);

		m_Texture = std::make_unique<Texture>(path);
	}
}
