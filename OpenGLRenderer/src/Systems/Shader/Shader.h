#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "../Utils/Action.h"

class Shader
{
public:
	const unsigned int Id;
	const std::string Path;

private:
	std::unordered_map<std::string, int> uniformLocationCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// Set Uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, int* value, unsigned int size);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	GLint GetUniformLocation(const std::string& name);
};