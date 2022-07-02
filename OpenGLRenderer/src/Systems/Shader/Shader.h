#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

class Shader
{
private:
	friend class ShaderManager;

public:
	const unsigned int Id;
	const std::string Path;

private:
	std::unordered_map<std::string, int> m_UniformLocationCache;
	unsigned int m_RefCount;

private:
	Shader(unsigned int id, const std::string& filePath);
	~Shader();

public:
	void Bind() const;
	void UnBind() const;

	// Set Uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, int* value, unsigned int size);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1fv(const std::string& name, float* value, unsigned int  index);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	GLint GetUniformLocation(const std::string& name);
};