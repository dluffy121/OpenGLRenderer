#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Logger/Logger.h>

using namespace core;

Shader::Shader(unsigned int id, const std::string& filePath) :
	Id(id),
	Path(filePath)
{}

Shader::~Shader()
{
	GLLog(glDeleteProgram(Id));
	std::cout << "Deleted Shader with id: " << Id << std::endl;
}

void Shader::Bind() const
{
	GLLog(glEnable(GL_BLEND));
	GLLog(glBlendEquation(GL_FUNC_ADD));
	GLLog(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	ASSERT(glIsProgram(Id) == GL_TRUE);
	GLLog(glUseProgram(Id));
}

void Shader::UnBind() const
{
	GLLog(glDisable(GL_BLEND));

	GLLog(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLLog(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string& name, int* value, unsigned int size)
{
	GLLog(glUniform1iv(GetUniformLocation(name), size, value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLLog(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1fv(const std::string& name, float* value, unsigned int size)
{
	GLLog(glUniform1fv(GetUniformLocation(name), size, value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLLog(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLLog(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLLog(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLLog(GLint location = glGetUniformLocation(Id, name.c_str()));
	m_UniformLocationCache[name] = location;
	return location;
}