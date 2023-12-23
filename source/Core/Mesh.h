#pragma once
#include <string>
#include <vector>
#include "Rendering/Vertex.h"
#include <glm.hpp>

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, glm::mat4 transform);

	const float* GetVertexData() const { return (float*)m_Vertices.data(); }
	const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
	size_t GetVertexDataSize() const { return m_Vertices.size() * sizeof(Vertex); }
	size_t GetVertexCount() const { return m_Vertices.size(); }

	const uint32_t* GetIndexData() const { return m_Indices.data(); }
	const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
	size_t GetIndexDataSize() const { return m_Indices.size() * sizeof(uint32_t); }
	size_t GetIndexCount() const { return m_Indices.size(); }
	const glm::mat4& GetTransformMatrix() const { return m_Transform; }

private:
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
	glm::mat4 m_Transform;
};