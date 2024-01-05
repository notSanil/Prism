#include "pch.h"
#include "ColladaReader.h"
#include <rapidxml_utils.hpp>
#include "Core/Mesh.h"
#include "Rendering/Model.h"
#include <cstddef>
#include <span>

#define VERTICES_PER_TRIANGLE 3ul
#define ATTRIBUTES_PER_VERTEX 3ul

#define GEOMETRY_NODE "instance_geometry"


void FillTransform(rapidxml::xml_node<>* transformDataNode, glm::mat4& transform)
{
	std::stringstream dataStream(transformDataNode->value());
	for (int i = 0; i < 16; ++i)
		dataStream >> transform[i % 4][i / 4];
}

Model ColladaReader::ParseFile(const std::string& path)
{
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	/*
	Rough structure of file(See Collada Spec 1.4.1, page 68 for more details)

	GeometryLibarary
	|__ Geometry
	|	|__ Mesh
	|		|__ Source
	|		|__ Source
	|		|__ Source
	|		|__ Vertices
	|		|__ Polylist / Triangles / etc
	|__ Geometry
	*/
	auto geometryLibraryNode = doc.first_node()->first_node("library_geometries");
	std::unordered_map<std::string, GeometryData> geometryRepository;

	for (auto geometry = geometryLibraryNode->first_node(); geometry; geometry = geometry->next_sibling())
	{
		GeometryData data = ParseMeshNode(geometry->first_node());
		std::string geometryID = geometry->first_attribute()->value();

		geometryRepository[geometryID] = data;
	}

	Model model;
	auto transformRoot = doc.first_node()->first_node("library_visual_scenes");
	auto meshInstanceNode = transformRoot->first_node()->first_node();
	for (; meshInstanceNode; meshInstanceNode = meshInstanceNode->next_sibling())
	{
		glm::mat4 transform;
		auto meshTransformNode = meshInstanceNode->first_node();
		FillTransform(meshTransformNode, transform);

		auto geometryIDNode = meshTransformNode->next_sibling();
		std::string geometryIDUrl = geometryIDNode->first_attribute()->value();
		geometryIDUrl.erase(0, 1);

		GeometryData& geometry = geometryRepository[geometryIDUrl];
		Mesh mesh(geometry.Vertices, geometry.Indices, transform);

		model.AddMesh(mesh);
	}

	return model;
}

ColladaReader::GeometryData ColladaReader::ParseMeshNode(rapidxml::xml_node<>* meshNode)
{
	const std::string triangleNodeName = "triangles";
	const std::string sourceNodeName = "source";
	const std::string verticesNodeName = "vertices";

	std::vector<float> positions;

	std::unordered_map<std::string, std::pair<DataType, std::vector<std::byte>>> sourceNodes;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	
	auto currentNode = meshNode->first_node();
	while (currentNode && currentNode->name() == sourceNodeName)
	{
		sourceNodes.emplace(currentNode->first_attribute()->value(), std::make_pair(DataType::None, std::vector<std::byte>()));
		DataType type = CreateBufferFromSourceNode(currentNode, sourceNodes.at(currentNode->first_attribute()->value()).second);
		sourceNodes.at(currentNode->first_attribute()->value()).first = type;

		currentNode = currentNode->next_sibling();
	}

	while (currentNode && currentNode->name() == verticesNodeName)
	{
		std::string id = currentNode->first_node("input")->first_attribute("source")->value();
		id.erase(id.begin());

		positions = ConvertTo<float>(sourceNodes.at(id).second);

		currentNode = currentNode->next_sibling();
	}

	while (currentNode && currentNode->name() == triangleNodeName)
	{
		auto [v, i] = ParseTriangleNode(currentNode, positions, sourceNodes);
		vertices = v;
		indices = i;

		currentNode = currentNode->next_sibling();
	}

	return GeometryData{vertices, indices};
}

/*
Rough Structure of Source node(See collada spec 1.4.1, page 149 for more details)

Source
|__ Asset (Optional)
|__ Array (One of many types)
|__ Technique_Common
	|__ Accessor
		|__ Param
		|__ Param

*/
ColladaReader::DataType ColladaReader::CreateBufferFromSourceNode(rapidxml::xml_node<char>* node, std::vector<std::byte>& buffer)
{
	auto currentNode = node->first_node();
	if (std::strcmp(currentNode->name(), "asset") == 0)
	{
		currentNode = currentNode->next_sibling();
	}
	
	char* strCount = currentNode->first_attribute("count")->value();
	int count = atoi(strCount);
	
	auto arrayName = std::string(currentNode->name());
	arrayName.erase(arrayName.size() - 6);

	DataType dataType = StringToType(arrayName);
	buffer.resize(SizeOfType(dataType) * count);
	std::stringstream dataStream(currentNode->value());

	switch (dataType)
	{
	case ColladaReader::DataType::None:
		break;
	case ColladaReader::DataType::Name:
		//TODO: Implement this!!
		break;
	case ColladaReader::DataType::Bool:
		DeserializeArrayToBuffer(dataStream, reinterpret_cast<bool*>(buffer.data()), count);
		break;
	case ColladaReader::DataType::Float:
		DeserializeArrayToBuffer(dataStream, reinterpret_cast<float*>(buffer.data()), count);
		break;
	case ColladaReader::DataType::Int:
		DeserializeArrayToBuffer(dataStream, reinterpret_cast<int*>(buffer.data()), count);
		break;
	default:
		break;
	}

	while (std::strcmp(currentNode->name(), "technique_common") != 0 && currentNode)
		currentNode = currentNode->next_sibling();

	//TODO: Implement it so that we actually use the values in the accessor node to parse mesh data


	return dataType;
}

constexpr ColladaReader::DataType ColladaReader::StringToType(const std::string& name)
{
	if (name == "float")
		return DataType::Float;
	else if (name == "int")
		return DataType::Int;
	else if (name == "bool")
		return DataType::Bool;
	else if (name == "Name")
		return DataType::Name;
	
	return DataType::None;
}

constexpr size_t ColladaReader::SizeOfType(DataType t)
{
	switch (t)
	{
	case ColladaReader::DataType::None:
		return 0;
	case ColladaReader::DataType::Name:
		return 0;
	case ColladaReader::DataType::Bool:
		return sizeof(bool);
	case ColladaReader::DataType::Float:
		return sizeof(float);
	case ColladaReader::DataType::Int:
		return sizeof(int);
	default:
		return 0;
	}
}

/*
Rough structure of triangle(See collada spec 1.4.1, page 160 for more details)

Triangle
|__ Input
|__ Input
|__ Input
|__ p

*/
std::pair<std::vector<Vertex>, std::vector<uint32_t>> ColladaReader::ParseTriangleNode(rapidxml::xml_node<char>* node, std::vector<float>& position, std::unordered_map<std::string, std::pair<DataType, std::vector<std::byte>>>& buffers)
{
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec2> vertexUvs;

	auto currentNode = node->first_node();
	int8_t vertexOffset = -1, normalOffset = -1, uvOffset = -1;
	uint32_t triangleCount = std::atoi(node->first_attribute("count")->value());

	while (currentNode && std::strcmp(currentNode->name(), "input") == 0)
	{
		std::string semantic = currentNode->first_attribute("semantic")->value();
		std::string source = currentNode->first_attribute("source")->value();
		source.erase(source.begin());
		int8_t offset = std::atoi(currentNode->first_attribute("offset")->value());

		if (semantic == "VERTEX")
		{
			vertexOffset = offset;
			vertexPositions = std::vector<glm::vec3>(position.size() / 3);

			for (size_t i = 0; i < vertexPositions.size(); ++i)
			{
				vertexPositions[i] = { position[i * 3], position[i * 3 + 1], position[i * 3 + 2] };
			}
		}
		else if (semantic == "NORMAL")
		{
			normalOffset = offset;
			vertexNormals = ConvertTo<glm::vec3>(buffers.at(source).second);
		}
		else if (semantic == "TEXCOORD")
		{
			uvOffset = offset;
			vertexUvs = ConvertTo<glm::vec2>(buffers.at(source).second);
		}

		currentNode = currentNode->next_sibling();
	}

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indexData;
	std::unordered_map<std::string, int> vertexIds;
	if (std::strcmp(currentNode->name(), "p") == 0)
	{
		std::vector<std::byte> buffer{ triangleCount * VERTICES_PER_TRIANGLE * ATTRIBUTES_PER_VERTEX * sizeof(int)};
		std::stringstream dataStream(currentNode->value());
		DeserializeArrayToBuffer(dataStream, reinterpret_cast<int*>(buffer.data()), triangleCount * VERTICES_PER_TRIANGLE * ATTRIBUTES_PER_VERTEX);
		std::vector<int> indices = ConvertTo<int>(buffer);

		const size_t totalIndices = triangleCount * VERTICES_PER_TRIANGLE;

		indexData = std::vector<uint32_t>(totalIndices);

		uint32_t currentId = 0;
		for (size_t i = 0; i < totalIndices; ++i)
		{
			std::string vertexId = std::to_string(indices[i * VERTICES_PER_TRIANGLE + vertexOffset]) + '-' +
				std::to_string(indices[i * VERTICES_PER_TRIANGLE + normalOffset]) + '-' +
				std::to_string(indices[i * VERTICES_PER_TRIANGLE + uvOffset]);

			if (vertexIds.count(vertexId) == 0)
			{
				vertices.push_back({});
				vertices.back().Position =	vertexPositions[indices[i * VERTICES_PER_TRIANGLE + vertexOffset]];
				vertices.back().Normal =	vertexNormals[	indices[i * VERTICES_PER_TRIANGLE + normalOffset]];
				vertices.back().UV =		vertexUvs[		indices[i * VERTICES_PER_TRIANGLE + uvOffset]];
				vertexIds[vertexId] = currentId;
				++currentId;
			}

			indexData[i] = vertexIds[vertexId];
		}
	}

	return { vertices, indexData };
}
