#include "ShaderAsset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include "../../Core/Logger.h"

using namespace core;

ShaderAsset::ShaderAsset(const std::string& shaderPath) :
	shaderPath(shaderPath),
	VertexShader(0),
	FragmentShader(0)
{
	programSource = ParseShader();
}

ShaderAsset::~ShaderAsset()
{
}

void ShaderAsset::Compile()
{
	VertexShader = CompileShader(GL_VERTEX_SHADER, programSource.VertexSource);
	FragmentShader = CompileShader(GL_FRAGMENT_SHADER, programSource.FragmentSource);
}

ShaderProgramSource ShaderAsset::ParseShader()
{
	std::ifstream stream(shaderPath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return
	{
		ss[0].str(),
		ss[1].str()
	};
}

unsigned int ShaderAsset::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLLog(glShaderSource(id, 1, &src, NULL));
	GLLog(glCompileShader(id));

	int result;
	GLLog(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLLog(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLLog(glGetShaderInfoLog(id, length, &length, message));

		std::cerr << "Failed to Compile Shader with id: " << id << " type: " << type << std::endl;
		std::cerr << message << std::endl;

		GLLog(glDeleteShader(id));

		return 0;
	}

	return id;
}