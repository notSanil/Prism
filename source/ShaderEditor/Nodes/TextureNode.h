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

private:
	std::unique_ptr<Texture> m_Texture = nullptr;
};

