#include "pch.h"
#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, glm::mat4 transform)
	: m_Vertices(vertices),
	  m_Indices(indices),
	  m_Transform(transform)
{}