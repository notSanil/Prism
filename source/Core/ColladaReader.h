#pragma once
#include <rapidxml.hpp>
#include <string>
#include "Core/Mesh.h"
#include "Rendering/Vertex.h"
#include "Rendering/Model.h"


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

	static GeometryData ParseGeometryNode(rapidxml::xml_node<>* geometryNode, 
		const std::string& geometryID);
	//TODO: Refactor this to reduce the argument quantity
	static void CreateIndices(rapidxml::xml_node<char>* dataNode, const std::vector<float>& positions,
		const std::vector<float>& normals, const std::vector<float>& uvs,
		std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void FillBufferFromNode(rapidxml::xml_node<char>* node, std::vector<float>& buffer);
};

