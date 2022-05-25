#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../Core/Logger.h"

using namespace core;

Shader::Shader(unsigned int shaderId) :
	shaderId(shaderId)
{}

Shader::~Shader()
{
	GLLog(glDeleteProgram(shaderId));
	std::cout << "Deleted Shader with id: " << shaderId << std::endl;
}

void Shader::Bind() const
{
	GLLog(glEnable(GL_BLEND));
	GLLog(glBlendEquation(GL_FUNC_ADD));
	GLLog(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	ASSERT(glIsProgram(shaderId) == GL_TRUE);
	GLLog(glUseProgram(shaderId));
}

void Shader::UnBind() const
{
	GLLog(glDisable(GL_BLEND));

	GLLog(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLLog(glUniform1i(GetUniformLocation(name), value));

	UpdateAction();
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLLog(glUniform1f(GetUniformLocation(name), value));

	UpdateAction();
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLLog(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));

	UpdateAction();
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLLog(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));

	UpdateAction();
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];

	GLLog(GLint location = glGetUniformLocation(shaderId, name.c_str()));
	uniformLocationCache[name] = location;
	return location;
}

void Shader::UpdateAction()
{
	for (auto& action : OnUpdate)
		action();
}
