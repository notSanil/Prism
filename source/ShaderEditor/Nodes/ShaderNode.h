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

private:
	std::unique_ptr<Shader> m_Shader;
};

