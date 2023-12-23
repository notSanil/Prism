#pragma once
#include <vector>
#include "Core/Mesh.h"
#include "Rendering/VertexAttributeObject.h"


class Model
{
public:
	Model() = default;
	void AddMesh(Mesh mesh);
	static Model FromCollada(const std::string& filePath);
	void Compile();

	size_t GetMeshCount() const { return m_Meshes.size(); }

	uint32_t GetVertexCount() const { return m_VertexCount; }
	uint32_t GetIndexCount() const { return m_IndexCount; }
	uint32_t GetTriangleCount() const { return m_IndexCount / 3; }
	inline VertexAttributeObject& GetVao() { return m_Vao; }
	inline Buffer& GetShaderStorageBuffer() { return m_ShaderStorage; }
	inline void BindVao() { m_Vao.Bind(); }
	inline void BindShaderStorageBuffer() { m_ShaderStorage.Bind(0); }

private:
	std::vector<Mesh> m_Meshes;
	VertexAttributeObject m_Vao;
	Buffer m_VertexBuffer, m_IndexBuffer;
	Buffer m_ShaderStorage;

	uint32_t m_VertexCount = 0;
	uint32_t m_IndexCount = 0;
};

