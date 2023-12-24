#pragma once
#include <Events/Event.h>
#include "ShaderEditor/NodeManager.h"
#include <imgui.h>
#include <glm.hpp>
#include <Core/Input.h>
#include "ShaderEditor/Explorer.h"

class ShaderEditor
{
public:
	ShaderEditor();;
	~ShaderEditor();

	void Render();
	void OnEvent(const Event& e);
	void GuiRender();

private:
	template<class T>
	void RenderSelectableNode()
	{
		if (ImGui::Selectable(T::GetName()))
		{
			m_NodeManager.AddNode<T>({ Input::GetMouseX(), Input::GetMouseY() });
		}
	}

private:
	NodeManager m_NodeManager;
	Explorer m_Explorer;
};

