#include "pch.h"
#include "NodeManager.h"
#include <imnodes.h>
#include "Nodes/Attributes.h"

void NodeManager::AddLink(int outputAttrId, int inputAttrId)
{
	int destNodeId = GetNodeIdFromInput(inputAttrId);
	int inputAttrIndex = GetAttrIdFromInput(inputAttrId);

	int sourceNodeId = GetNodeIdFromOutput(outputAttrId);
	int outputAttrIndex = GetAttrIdFromOutput(outputAttrId);

	const OutputAttribute* sourceAttribute = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&](const auto& node)
		{
			return node->GetId() == sourceNodeId;
		}).operator*()->GetOutputAttribute(outputAttrIndex);

	DataType sourceAttributeType = sourceAttribute->Type;

	InputAttribute* destAttribute = std::find_if(m_Nodes.begin(), m_Nodes.end(), [destNodeId](const auto& node)
		{
			return node->GetId() == destNodeId;
		}).operator*()->GetInputAttribute(inputAttrIndex);

	DataType destAttributeType = destAttribute->Type;

	if (destAttributeType == sourceAttributeType)
	{
		m_Links.emplace_back(outputAttrId, inputAttrId);

		destAttribute->Value = sourceAttribute->Value;
	}
}

void NodeManager::DeleteLink(int id)
{
	auto it = m_Links.begin() + id;
	auto& link = *it;

	int destNodeId = GetNodeIdFromInput(link.InputAttrIndex);
	std::unique_ptr<Node>& destAttribute = *std::find_if(m_Nodes.begin(), m_Nodes.end(), [destNodeId](const auto& node)
		{
			return node->GetId() == destNodeId;
		});

	destAttribute->GetInputAttribute(GetAttrIdFromInput(link.InputAttrIndex))->Value = nullptr;

	m_Links.erase(it);
}

void NodeManager::Render()
{
	for (auto& node : m_Nodes)
		node->Render();
}

void NodeManager::GuiRender()
{
	for (auto& node : m_Nodes)
	{
		node->GuiRender();
	}

	int i = 0;
	for (auto& link : m_Links)
	{
		ImNodes::Link(i, link.OutputAttrIndex, link.InputAttrIndex);
		++i;
	}
}

int NodeManager::GetNodeIdFromOutput(int id)
{
	return id >> 16;
}

int NodeManager::GetNodeIdFromInput(int id)
{
	return id >> 8;
}

int NodeManager::GetAttrIdFromInput(int id)
{
	return id & 0xFF;
}

int NodeManager::GetAttrIdFromOutput(int id)
{
	return (id & 0xFF00) >> 8;
}
