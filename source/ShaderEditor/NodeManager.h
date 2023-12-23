#pragma once
#include "ShaderEditor/Nodes/Node.h"
#include <vector>
#include <utility>
#include <glm.hpp>
#include <imnodes.h>

class NodeManager
{
public:
	NodeManager() = default;
	~NodeManager() = default;

	template<typename T, typename... Args>
	T& AddNode(const glm::vec2& position, Args&&... args)
	{
		m_Nodes.push_back(std::make_unique<T>(static_cast<int>(m_Nodes.size()) + 1, std::forward<Args>(args)...));
		ImNodes::SetNodeScreenSpacePos(static_cast<int>(m_Nodes.size()), reinterpret_cast<const ImVec2&>(position));
		return static_cast<T&>(*m_Nodes.back());
	}

	void AddLink(int outputAttrId, int inputAttrId);
	void DeleteLink(int id);
	
	void Render();
	void GuiRender();

private:
	int GetNodeIdFromOutput(int id);
	int GetNodeIdFromInput(int id);
	int GetAttrIdFromInput(int id);
	int GetAttrIdFromOutput(int id);


	struct Link
	{
		int OutputAttrIndex;
		int InputAttrIndex;
	};

private:
	std::vector<std::unique_ptr<Node>> m_Nodes;
	std::vector<Link> m_Links;
};

