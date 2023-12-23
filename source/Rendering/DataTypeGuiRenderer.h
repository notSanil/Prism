#pragma once
#include "Rendering/GlslDataTypes.h"
#include <string>
#include <imgui.h>
#include <glm.hpp>

template<DataType T, typename U>
void RenderGui(U& uniform, const std::string& name)
{
	ImGui::Text(name.c_str());
	ImGui::SameLine();
	ImGui::Text(DataTypeToString.at(static_cast<int>(T)).c_str());
}

void RenderUniformGui(DataType type, void* uniform, const std::string& name);
