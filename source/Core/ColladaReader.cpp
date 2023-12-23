#include "pch.h"
#include "ColladaReader.h"
#include <rapidxml_utils.hpp>
#include "Core/Mesh.h"
#include "Rendering/Model.h"

#define VERTICES_PER_TRIANGLE 3
#define ATTRIBUTES_PER_VERTEX 3

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

	auto geometriesRoot = doc.first_node()->first_node("library_geometries");
	std::unordered_map<std::string, GeometryData> geometryRepository;
	for (auto meshGeometryNode = geometriesRoot->first_node(); meshGeometryNode; meshGeometryNode = meshGeometryNode->next_sibling())
	{
		GeometryData data = ParseGeometryNode(meshGeometryNode->first_node(), 
			meshGeometryNode->first_attribute()->value());
		std::string geometryID = meshGeometryNode->first_attribute()->value();

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

ColladaReader::GeometryData ColladaReader::ParseGeometryNode(rapidxml::xml_node<>* geometryNode, const std::string& geometryID)
{
	const std::string triangleNodeName = "triangles";

	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> uvs;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (auto dataNode = geometryNode->first_node(); dataNode; dataNode = dataNode->next_sibling())
	{
		if (dataNode->first_attribute()->value() == geometryID + "-positions")
		{
			FillBufferFromNode(dataNode->first_node(), positions);
		}
		else if (dataNode->first_attribute()->value() == geometryID + "-normals")
		{
			FillBufferFromNode(dataNode->first_node(), normals);
		}
		else if (dataNode->first_attribute()->value() == geometryID + "-map-0")
		{
			FillBufferFromNode(dataNode->first_node(), uvs);
		}
		else if (dataNode->name() == triangleNodeName)
		{
			CreateIndices(dataNode->first_node("p"), positions, normals, uvs, vertices, indices);
		}
	}

	return GeometryData{vertices, indices};
}

void ColladaReader::FillBufferFromNode(rapidxml::xml_node<char>* node, std::vector<float>& buffer)
{
	char* strCount = node->first_attribute("count")->value();
	int count = atoi(strCount);

	buffer.resize(count, 0.0f);

	std::stringstream dataStream(node->value());
	for (int i = 0; i < count; ++i)
		dataStream >> buffer[i];	
}

void ColladaReader::CreateIndices(rapidxml::xml_node<char>* dataNode,
	const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& uvs,
	std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	char* strCount = dataNode->parent()->first_attribute("count")->value();
	size_t triangleCount = atoi(strCount);

	const size_t totalVertices = triangleCount * VERTICES_PER_TRIANGLE;
	const size_t totalAttributes = totalVertices * ATTRIBUTES_PER_VERTEX;

	std::vector<size_t> triangleData(totalAttributes, 100);

	{
		std::stringstream data(dataNode->value());
		for (size_t i = 0; i < triangleData.size(); ++i)
			data >> triangleData[i];
	}

	indices.reserve(triangleCount * VERTICES_PER_TRIANGLE);
	std::unordered_map<std::string, int> foundIndexes;
	int currentPos = 0;
	for (size_t i = 0; i < totalAttributes; i += ATTRIBUTES_PER_VERTEX)
	{
		size_t positionIndex = triangleData[i + 0] * 3;
		size_t normalIndex	 = triangleData[i + 1] * 3;
		size_t texIndex		 = triangleData[i + 2] * 2;

		std::string vertexID = std::to_string(positionIndex) + '-' + std::to_string(normalIndex) +
			'-' + std::to_string(texIndex);

		if (foundIndexes.count(vertexID) == 0)
		{
			foundIndexes[vertexID] = currentPos;

			Vertex toBeAdded;
			
			toBeAdded.Position.x = positions[positionIndex + 0ul];
			toBeAdded.Position.y = positions[positionIndex + 1ul];
			toBeAdded.Position.z = positions[positionIndex + 2ul];

			toBeAdded.Normal.x = normals[normalIndex + 0ul];
			toBeAdded.Normal.y = normals[normalIndex + 1ul];
			toBeAdded.Normal.z = normals[normalIndex + 2ul];

			toBeAdded.UV.x = uvs[texIndex + 0ul];
			toBeAdded.UV.y = uvs[texIndex + 1ul];

			vertices.push_back(toBeAdded);
			++currentPos;
		}

		indices.push_back(foundIndexes[vertexID]);
	}
}
