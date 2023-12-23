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

	ImGui::Begin("simple node editor");
	
	ImNodes::BeginNodeEditor();
	bool editorHovered = ImNodes::IsEditorHovered();
	bool windowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);		
	m_NodeManager.GuiRender();
	ImNodes::MiniMap();
	ImNodes::EndNodeEditor();

	if (editorHovered && windowFocused)
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("my_select_popup");
		}
	}

	if (ImGui::BeginPopup("my_select_popup"))
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
	ImGui::End();

	//ImGui::ShowDemoWindow();
}