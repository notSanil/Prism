#pragma once
#include <filesystem>
#include <Rendering/Texture.h>

class Explorer
{
public:
	Explorer();
	~Explorer() = default;

	void GuiRender();


private:
	std::filesystem::path m_CurrentDirectory;
	Texture m_FileIcon, m_DirectoryIcon;

	static constexpr float c_ImageSize = 100.0f;
};

