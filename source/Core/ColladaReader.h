#pragma once
#include <rapidxml.hpp>
#include <string>
#include "Core/Mesh.h"
#include "Rendering/Vertex.h"
#include "Rendering/Model.h"
#include <span>


class ColladaReader
{
public:
	static Model ParseFile(const std::string& path);

private:
	struct GeometryData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
	};

	enum class DataType
	{
		None,
		Name,
		Bool,
		Float,
		Int
	};

	static GeometryData ParseMeshNode(rapidxml::xml_node<>* geometryNode);
	static DataType CreateBufferFromSourceNode(rapidxml::xml_node<char>* node, std::vector<std::byte>& buffer);
	static std::pair<std::vector<Vertex>, std::vector<uint32_t>> ParseTriangleNode(rapidxml::xml_node<char>* node, std::vector<float>& position, std::unordered_map<std::string, std::pair<DataType, std::vector<std::byte>>>& buffers);

	constexpr static DataType StringToType(const std::string& name);
	constexpr static size_t SizeOfType(DataType t);

	template<typename T>
	constexpr static std::vector<T> ConvertTo(std::vector<std::byte>& buffer)
	{
		std::vector<T> returnBuffer(buffer.size() / sizeof(T));
		std::copy_n(buffer.begin(), returnBuffer.size() * sizeof(T),
			std::as_writable_bytes(std::span(returnBuffer)).begin());
		return returnBuffer;
	}

	template<typename T>
	static void DeserializeArrayToBuffer(std::stringstream& stream, T* buffer, int elementCount)
	{
		for (int i = 0; i < elementCount; ++i)
			stream >> buffer[i];
	}


};

