#include "pch.h"
#include "FileNode.h"
#include <imnodes.h>
#include <glm.hpp>
#include "Attributes.h"

std::unique_ptr<Texture> FileNode::s_Icon = nullptr;

FileNode::FileNode(int id)
	:Node(id)
{
	m_SourcePath.reserve(64);

	m_OutputAttribute = { DataType::File, &m_SourcePath, "" };

	if (!s_Icon)
		s_Icon = std::make_unique<Texture>("Resources/Textures/File.png");
}

FileNode::~FileNode()
{}

void FileNode::Render()
{
	
}

void FileNode::GuiRender()
{
	ImNodes::BeginNode(m_Id);
	
	ImNodes::BeginNodeTitleBar();
	ImGui::Text(GetName());
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginStaticAttribute(c_StaticStart + 0);
	ImVec2 imageSize = { 50, 50 };
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imageSize.x * 0.5f);
	ImGui::Image(s_Icon->GetRendererID(), imageSize);
	ImNodes::EndStaticAttribute();

	ImNodes::BeginOutputAttribute(c_OutputStart + 0);
	ImGui::PushItemWidth(100);
	ImGui::InputText("##Text", m_SourcePath.data(), m_SourcePath.capacity());
	ImGui::PopItemWidth();
	ImNodes::EndOutputAttribute();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH"))
		{
			const char* data = static_cast<const char*>(payload->Data);
			m_SourcePath = std::string(data, payload->DataSize);
		}
		ImGui::EndDragDropTarget();
	}

	ImNodes::EndNode();
}

