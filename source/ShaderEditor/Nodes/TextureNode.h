#pragma once
#include "Node.h"
#include "Rendering/Texture.h"
#include "ShaderEditor/Nodes/Attributes.h"

class TextureNode : public Node
{
public:
	TextureNode(int id);
	~TextureNode() override;

	NODE(Texture)
	void GuiRender() override;
	void Render() override;

	virtual const InputAttribute* GetInputAttribute(int idx) const override { return &m_InputAttribute; }
	virtual InputAttribute* GetInputAttribute(int idx) override { return &m_InputAttribute; }
	virtual const OutputAttribute* GetOutputAttribute(int idx) const override { return &m_OutputAttribute; }

private:
	std::unique_ptr<Texture> m_Texture = nullptr;
	OutputAttribute m_OutputAttribute;
	InputAttribute m_InputAttribute;
};

