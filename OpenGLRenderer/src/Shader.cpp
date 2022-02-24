#include "Shader.h"
#include "OpenGLLogger.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
	ASSERT(glIsProgram(shaderId) == GL_TRUE);
	GLLog(glUseProgram(shaderId));
}

void Shader::UnBind() const
{
	GLLog(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLLog(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];

	GLLog(GLint location = glGetUniformLocation(shaderId, name.c_str()));
	uniformLocationCache[name] = location;
	return location;
}
