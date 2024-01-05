#pragma once
#include "Node.h"
#include "Rendering/Texture.h"
#include "ShaderEditor/Nodes/Attributes.h"

class FileNode : public Node
{
public:
	FileNode(int id);
	~FileNode();

	NODE(File)

	void Render() override;
	void GuiRender() override;
	inline const std::string& GetSourcePath() const { return m_SourcePath; };
	inline void SetSourcePath(std::string path) { m_SourcePath = path; }
private:
	std::string m_SourcePath;
	
	static std::unique_ptr<Texture> s_Icon;
};

