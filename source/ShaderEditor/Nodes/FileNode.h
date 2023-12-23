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
	virtual const OutputAttribute* GetOutputAttribute(int idx) const override { return &m_OutputAttribute; };
	virtual const InputAttribute* GetInputAttribute(int idx) const override { return nullptr; };
	virtual InputAttribute* GetInputAttribute(int idx) override { return nullptr; };
private:
	std::string m_SourcePath;
	OutputAttribute m_OutputAttribute;
	
	static std::unique_ptr<Texture> s_Icon;
};

