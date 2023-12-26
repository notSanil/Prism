#include "pch.h"
#include "ShaderEditor.h"
#include <imgui.h>
#include <imnodes.h>
#include "Nodes/ShaderNode.h"
#include "Nodes/FileNode.h"
#include "Nodes/TextureNode.h"
#include "Nodes/Vec4Node.h"
#include "Nodes/Vec3Node.h"

ShaderEditor::ShaderEditor()
{
	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
}

ShaderEditor::~ShaderEditor()
{
}


void ShaderEditor::Render()
{
	m_NodeManager.Render();
}

void ShaderEditor::OnEvent(const Event& e)
{
}

void ShaderEditor::GuiRender()
{
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Editor");
	
	ImNodes::BeginNodeEditor();
	bool editorHovered = ImNodes::IsEditorHovered();
	bool windowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);		
	m_NodeManager.GuiRender();
	ImNodes::MiniMap();
	ImNodes::EndNodeEditor();

	if (editorHovered)
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("Node Add Popup");
		}
	}

	if (ImGui::BeginPopup("Node Add Popup"))
	{
		RenderSelectableNode<FileNode>();
		RenderSelectableNode<ShaderNode>();
		RenderSelectableNode<TextureNode>();
		RenderSelectableNode<Vec4Node>();
		RenderSelectableNode<Vec3Node>();

		ImGui::EndPopup();
	}
	

	int startId, endId;
	if (ImNodes::IsLinkCreated(&startId, &endId))
	{
		m_NodeManager.AddLink(startId, endId);
	}

	int id;
	if (ImNodes::IsLinkDestroyed(&id))
	{
		m_NodeManager.DeleteLink(id);
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("FILE_PATH"))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH"))
			{
				const char* data = static_cast<const char*>(payload->Data);
				std::string path = std::string(data, payload->DataSize);
				FileNode& node = m_NodeManager.AddNode<FileNode>({ Input::GetMouseX(), Input::GetMouseY() });
				node.SetSourcePath(path);
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::End();

	m_Explorer.GuiRender();


	//ImGui::ShowDemoWindow();
}