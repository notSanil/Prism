#include <pch.h>
#include "DataTypeGuiRenderer.h"
#include <gtc/type_ptr.hpp>

template<>
void RenderGui<DataType::Float>(float& uniform, const std::string& name)
{
	ImGui::DragFloat(name.c_str(), &uniform);
}

template<>
void RenderGui<DataType::Float2>(glm::vec2& uniform, const std::string& name)
{
	ImGui::DragFloat2(name.c_str(), glm::value_ptr(uniform));
}

template<>
void RenderGui<DataType::Float3>(glm::vec3& uniform, const std::string& name)
{
	ImGui::DragFloat3(name.c_str(), glm::value_ptr(uniform));
}

template<>
void RenderGui<DataType::Float4>(glm::vec4& uniform, const std::string& name)
{
	ImGui::DragFloat4(name.c_str(), glm::value_ptr(uniform));
}

void RenderUniformGui(DataType type, void* uniform, const std::string& name)
{
	switch (type)
	{
	case DataType::Float:
		RenderGui<DataType::Float>(*static_cast<float*>(uniform), name);
		break;
	case DataType::Float2:
		RenderGui<DataType::Float2>(*static_cast<glm::vec2*>(uniform), name);
		break;
	case DataType::Float3:
		RenderGui<DataType::Float3>(*static_cast<glm::vec3*>(uniform), name);
		break;
	case DataType::Float4:
		RenderGui<DataType::Float4>(*static_cast<glm::vec4*>(uniform), name);
		break;
	case DataType::Int:
		RenderGui<DataType::Int>(*static_cast<int*>(uniform), name);
		break;
	case DataType::Int2:
		RenderGui<DataType::Int2>(*static_cast<glm::ivec2*>(uniform), name);
		break;
	case DataType::Int3:
		RenderGui<DataType::Int3>(*static_cast<glm::ivec3*>(uniform), name);
		break;
	case DataType::Int4:
		RenderGui<DataType::Int4>(*static_cast<glm::ivec4*>(uniform), name);
		break;
	case DataType::Bool:
		RenderGui<DataType::Bool>(*static_cast<bool*>(uniform), name);
		break;
	case DataType::Bool2:
		RenderGui<DataType::Bool2>(*static_cast<glm::bvec2*>(uniform), name);
		break;
	case DataType::Bool3:
		RenderGui<DataType::Bool3>(*static_cast<glm::bvec3*>(uniform), name);
		break;
	case DataType::Bool4:
		RenderGui<DataType::Bool4>(*static_cast<glm::bvec4*>(uniform), name);
		break;
	case DataType::Mat2:
		RenderGui<DataType::Mat2>(*static_cast<glm::mat2*>(uniform), name);
		break;
	case DataType::Mat3:
		RenderGui<DataType::Mat3>(*static_cast<glm::mat3*>(uniform), name);
		break;
	case DataType::Mat4:
		RenderGui<DataType::Mat4>(*static_cast<glm::mat4*>(uniform), name);
		break;
	case DataType::Sampler1D:
		break;
	case DataType::Sampler2D:
		break;
	case DataType::Sampler3D:
		break;
	default:
		break;
	}
}

