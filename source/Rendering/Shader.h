#pragma once
#include <string>
#include <glm.hpp>
#include "Rendering/GlslDataTypes.h"


struct VariableInfo
{
	DataType Type;
	int Location;
};

struct SsboInfo
{
	int BindingPoint;
	int ActiveVariableCount;
};

class Shader
{
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();
	Shader(const Shader& other) = delete;
	Shader(Shader&& other) noexcept;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) noexcept;

	void Bind();
	void Unbind();
	void UploadMat4Uniform(int location, const glm::mat4& value);
	void UploadVec3Uniform(int location, const glm::vec3& value);
	void UploadFloatUniform(int location, float value);
	void UploadIntUniform(int location, int value);
	
	void UploadMat4Uniform(const std::string& uniform, const glm::mat4& value);
	void UploadVec3Uniform(const std::string& uniform, const glm::vec3& value);
	void UploadFloatUniform(const std::string& uniform, float value);
	void UploadIntUniform(const std::string& uniform, int value);

	int GetUniformLocation(const std::string& uniform) const;
	int GetShaderStorageBindingPoint(const std::string& name) const;
	int GetSamplerBindPoint(const std::string& name) const;
	const std::unordered_map<std::string, VariableInfo>& GetUniformInfo() const { return m_UniformInfo; }
	const std::unordered_map<std::string, SsboInfo>& GetShaderStorageInfo() const { return m_ShaderStorageBlockInfo; }

private:
	void Introspect();
	void IntrospectUniforms();
	void IntrospectSSBOs();

	void AssignBindPointsToSamplers();

private:
	uint32_t m_RendererId = 0;

	std::unordered_map<std::string, VariableInfo> m_UniformInfo;
	std::unordered_map<std::string, SsboInfo> m_ShaderStorageBlockInfo;
	std::unordered_map<std::string, int> m_TextureBindPoints;
};

