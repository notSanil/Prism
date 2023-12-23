#include "pch.h"
#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <gtc/type_ptr.hpp>
#include <array>


Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		std::ifstream vertexFile(vertexShaderPath.c_str());
		if (!vertexFile)
		{
			LOG_ERROR("File not found! " << vertexShaderPath.c_str());
		}
		std::stringstream vertexBuffer;
		vertexBuffer << vertexFile.rdbuf();
		std::string vertexShaderSource = vertexBuffer.str();

		const char* source = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
	}
	glCompileShader(vertexShader);

	int isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(vertexShader);

		LOG_ERROR(infoLog.data());
		m_RendererId = 0;
		return;
	}

	
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		std::ifstream fragmentFile(fragmentShaderPath.c_str());
		if (!fragmentFile)
		{
			LOG_ERROR("File not found! " << fragmentShaderPath.c_str());
		}
		std::stringstream fragmentBuffer;
		fragmentBuffer << fragmentFile.rdbuf();
		std::string fragmentShaderSource = fragmentBuffer.str();

		const char* source = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);
	}

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		
		LOG_ERROR(infoLog.data());
		m_RendererId = 0;
		return;
	}

	
	m_RendererId = glCreateProgram();

	glAttachShader(m_RendererId, vertexShader);
	glAttachShader(m_RendererId, fragmentShader);

	glLinkProgram(m_RendererId);

	int isLinked = 0;
	glGetProgramiv(m_RendererId, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		int maxLength = 0;
		glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(m_RendererId);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		LOG_ERROR(infoLog.data());

		m_RendererId = 0;
		return;
	}

	glDetachShader(m_RendererId, vertexShader);
	glDetachShader(m_RendererId, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	Introspect();
	AssignBindPointsToSamplers();
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererId);
	m_RendererId = 0;
}

Shader::Shader(Shader&& other) noexcept
{
	*this = std::move(other);
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	std::swap(other.m_RendererId, m_RendererId);
	std::swap(other.m_UniformInfo, m_UniformInfo);
	std::swap(other.m_ShaderStorageBlockInfo, m_ShaderStorageBlockInfo);
	std::swap(other.m_TextureBindPoints, m_TextureBindPoints);

	return *this;
}

void Shader::Bind()
{
	glUseProgram(m_RendererId);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::UploadMat4Uniform(int location, const glm::mat4& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::UploadVec3Uniform(int location, const glm::vec3& value)
{
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::UploadFloatUniform(int location, float value)
{
	glUniform1fv(location, 1, &value);
}

void Shader::UploadIntUniform(int location, int value)
{
	glUniform1iv(location, 1, &value);
}

void Shader::UploadMat4Uniform(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::UploadVec3Uniform(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::UploadFloatUniform(const std::string& name, float value)
{
	glUniform1fv(GetUniformLocation(name), 1, &value);
}

void Shader::UploadIntUniform(const std::string& name, int value)
{
	glUniform1iv(GetUniformLocation(name), 1, &value);
}

DataType ConvertGLToInternal(int type)
{
	switch (type)
	{
	case GL_INT:
		return DataType::Int;
	case GL_INT_VEC2:
		return DataType::Int2;
	case GL_INT_VEC3:
		return DataType::Int3;
	case GL_INT_VEC4:
		return DataType::Int4;
	case GL_FLOAT:
		return DataType::Float;
	case GL_FLOAT_VEC2:
		return DataType::Float2;
	case GL_FLOAT_VEC3:
		return DataType::Float3;
	case GL_FLOAT_VEC4:
		return DataType::Float4;
	case GL_BOOL:
		return DataType::Bool;
	case GL_BOOL_VEC2:
		return DataType::Bool2;
	case GL_BOOL_VEC3:
		return DataType::Bool3;
	case GL_BOOL_VEC4:
		return DataType::Bool4;
	case GL_FLOAT_MAT2:
		return DataType::Mat2;
	case GL_FLOAT_MAT3:
		return DataType::Mat3;
	case GL_FLOAT_MAT4:
		return DataType::Mat4;
	case GL_SAMPLER_1D:
		return DataType::Sampler1D;
	case GL_SAMPLER_2D:
		return DataType::Sampler2D;
	case GL_SAMPLER_3D:
		return DataType::Sampler3D;
	default:
		return DataType::None;
	}
}

int Shader::GetUniformLocation(const std::string& uniform) const
{
	if (m_UniformInfo.count(uniform) == 0)
		return -1;

	return m_UniformInfo.at(uniform).Location;
}

int Shader::GetShaderStorageBindingPoint(const std::string& name) const
{
	if (m_ShaderStorageBlockInfo.count(name) == 0)
		return -1;

	return m_ShaderStorageBlockInfo.at(name).BindingPoint;
}

int Shader::GetSamplerBindPoint(const std::string& name) const
{
	if (m_TextureBindPoints.count(name) == 0)
		return -1;

	return m_TextureBindPoints.at(name);
}

void Shader::Introspect()
{
	IntrospectUniforms();
	IntrospectSSBOs();
}

void Shader::IntrospectUniforms()
{
	int numUniforms = 0;
	glGetProgramInterfaceiv(m_RendererId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	std::array<GLenum, 4> properties{ GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };
	for (int unif = 0; unif < numUniforms; ++unif)
	{
		std::array<GLint, 4> values;
		glGetProgramResourceiv(m_RendererId, GL_UNIFORM, unif, properties.size(), properties.data(), values.size(), NULL, values.data());

		if (values[0] != -1)
			continue;

		std::string name(static_cast<int>(values[2]), ' ');
		glGetProgramResourceName(m_RendererId, GL_UNIFORM, unif, name.size(), nullptr, name.data());
		name.pop_back(); //This is done to remove the null terminator at the end of the string
		//If we don't remove the terminator, the hashing and subsequent comparison fails

		m_UniformInfo[name] = { ConvertGLToInternal(values[1]), values[3] };
	}
}

void Shader::IntrospectSSBOs()
{
	int numBlocks = 0;
	glGetProgramInterfaceiv(m_RendererId, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);

	std::array<GLenum, 3> blockProperties{ GL_NUM_ACTIVE_VARIABLES, GL_BUFFER_BINDING, GL_NAME_LENGTH };
	std::array<GLenum, 1> activeVariableProperties{ GL_ACTIVE_VARIABLES };
	std::array<GLenum, 2> bufferVariableProperties{ GL_TYPE, GL_NAME_LENGTH };

	for (int blockIndex = 0; blockIndex < numBlocks; ++blockIndex)
	{
		std::array<int, 3> information;
		glGetProgramResourceiv(m_RendererId, GL_SHADER_STORAGE_BLOCK, blockIndex, blockProperties.size(),
			blockProperties.data(), information.size(), nullptr, information.data());

		int activeVariableCount = information[0];

		if (activeVariableCount == 0)
			continue;

		std::string blockName(information[2], ' ');
		glGetProgramResourceName(m_RendererId, GL_SHADER_STORAGE_BLOCK, blockIndex, blockName.size(), nullptr, blockName.data());
		blockName.pop_back();

		m_ShaderStorageBlockInfo[blockName] = { information[1], activeVariableCount };

		std::vector<int> activeVariableIndices(activeVariableCount);
		glGetProgramResourceiv(m_RendererId, GL_SHADER_STORAGE_BLOCK, blockIndex, activeVariableProperties.size(),
			activeVariableProperties.data(), activeVariableIndices.size(), nullptr, activeVariableIndices.data());

		for (auto variableIndex: activeVariableIndices)
		{
			std::array<int, 2> data;
			glGetProgramResourceiv(m_RendererId, GL_BUFFER_VARIABLE, variableIndex, bufferVariableProperties.size(),
				bufferVariableProperties.data(), data.size(), nullptr, data.data());

			std::string name(data[1], ' ');
			glGetProgramResourceName(m_RendererId, GL_BUFFER_VARIABLE, variableIndex, name.size(), nullptr, name.data());
			name.pop_back(); // See comment in IntrospectUniforms()
		}
	}
}

void Shader::AssignBindPointsToSamplers()
{
	Bind();
	int currentBindPoint = 0;
	for (auto [name, variableInfo] : m_UniformInfo)
	{
		if (variableInfo.Type == DataType::Sampler1D || variableInfo.Type == DataType::Sampler2D || variableInfo.Type == DataType::Sampler3D)
		{
			m_TextureBindPoints[name] = currentBindPoint;
			UploadIntUniform(name, currentBindPoint);
			++currentBindPoint;
		}
	}
	Unbind();
}
