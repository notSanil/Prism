#include "pch.h"
#include "Model.h"
#include "Core/ColladaReader.h"
#include "Rendering/Buffer.h"
#include <gtc/type_ptr.hpp>

struct IndexedVertex
{
	Vertex vertex;
	uint32_t index;

	IndexedVertex(Vertex v, uint32_t i)
		: vertex(v), index(i)
	{
	}
};


Model Model::FromCollada(const std::string& filePath)
{
	Model model = ColladaReader::ParseFile(filePath);
	model.Compile();

	return model;
}

void Model::AddMesh(Mesh mesh)
{
	m_Meshes.push_back(mesh);

	m_VertexCount += mesh.GetVertexCount();
	m_IndexCount += mesh.GetIndexCount();
}

void Model::Compile()
{
	std::vector<IndexedVertex> vertices;
	vertices.reserve(m_VertexCount);

	uint32_t transformIndex = 0;
	for (auto& mesh : m_Meshes)
	{
		for (const auto& vertex : mesh.GetVertices())
		{
			vertices.emplace_back(vertex, transformIndex);
		}

		++transformIndex;
	}

	std::vector<uint32_t> indices;
	indices.reserve(m_IndexCount);

	uint32_t offset = 0;
	for (auto& mesh : m_Meshes)
	{
		std::transform(mesh.GetIndices().begin(), mesh.GetIndices().end(), std::back_inserter(indices), [&](uint32_t element) {
			return offset + element;
		});
		offset = indices.size();
	}

	std::vector<glm::mat4> transforms;
	transforms.reserve(m_Meshes.size());
	for (auto& mesh : m_Meshes)
	{
		transforms.emplace_back(mesh.GetTransformMatrix());
	}

	m_VertexBuffer = Buffer(vertices.data(), vertices.size() * sizeof(IndexedVertex), BufferType::Vertex);
	m_IndexBuffer = Buffer(indices.data(), indices.size() * sizeof(uint32_t), BufferType::Index);

	m_ShaderStorage = Buffer(transforms.data(), transforms.size() * sizeof(glm::mat4), BufferType::ShaderStorage);

	m_Vao = VertexAttributeObject(m_VertexBuffer, m_IndexBuffer,
		{ {false, AttributeType::VEC3}, {false, AttributeType::VEC3}, {false, AttributeType::VEC2}, {false, AttributeType::INT} });
}