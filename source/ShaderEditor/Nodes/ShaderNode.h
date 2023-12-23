#pragma once
#include "ShaderEditor/Nodes/Node.h"
#include "ShaderEditor/Nodes/Attributes.h"
#include "Rendering/Shader.h"

class ShaderNode : public Node
{
public:
	ShaderNode(int id);
	~ShaderNode() override;

	NODE(Shader)

	void GuiRender() override;
	void Render() override;

	virtual const InputAttribute* GetInputAttribute(int idx) const override { return &m_InputAttributes.at(idx); }
	virtual const OutputAttribute* GetOutputAttribute(int idx) const override { return nullptr; }
	virtual InputAttribute* GetInputAttribute(int idx) override { return &m_InputAttributes.at(idx); };

private:
	std::vector<InputAttribute> m_InputAttributes;
	std::unique_ptr<Shader> m_Shader;
};

