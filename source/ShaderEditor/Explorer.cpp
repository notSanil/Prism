#include "pch.h"
#include "Explorer.h"
#include <imgui.h>

Explorer::Explorer()
{
	m_CurrentDirectory = "Resources";
	m_FileIcon = Texture("Resources/Textures/File.png");
	m_DirectoryIcon = Texture("Resources/Textures/Folder.png");
}

void Explorer::GuiRender()
{
	ImGui::Begin("Explorer");

	std::filesystem::path currentPath;
	for (auto it = m_CurrentDirectory.begin(); it != m_CurrentDirectory.end(); ++it)
	{
		currentPath /= *it;
		if (ImGui::Button(it->string().c_str()))
		{
			m_CurrentDirectory = currentPath;
		}
		ImGui::SameLine();
		ImGui::Text(">");
		ImGui::SameLine();
	}

	ImGui::NewLine();

	std::filesystem::path newDirectory;
	float width = ImGui::GetContentRegionAvail().x;
	int columnCount = width / (c_ImageSize + 100.0);

	if (ImGui::BeginTable("File Table", std::max(1, columnCount), ImGuiTableFlags_SizingFixedSame))
	{
		ImGui::TableNextRow();
		for (auto const& file : std::filesystem::directory_iterator{ m_CurrentDirectory })
		{
			ImGui::TableNextColumn();
			std::string name = file.path().string();
			if (file.is_directory())
			{

				if (ImGui::ImageButton(name.c_str(), m_DirectoryIcon.GetRendererID(), { c_ImageSize, c_ImageSize }))
					newDirectory = file.path();

			}
			else
			{
				ImGui::ImageButton(name.c_str(), m_FileIcon.GetRendererID(), { c_ImageSize, c_ImageSize });
			}
			if (file.is_regular_file())
			{
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("FILE_PATH", file.path().string().c_str(), file.path().string().size());
					ImGui::Text(file.path().string().c_str());
					ImGui::EndDragDropSource();
				}
			}
			ImGui::Text(file.path().filename().string().c_str());
		}
		ImGui::EndTable();
	}

	ImGui::End();

	if (!newDirectory.empty())
	{
		m_CurrentDirectory = newDirectory;
	}
}
