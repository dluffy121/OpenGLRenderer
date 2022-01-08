#pragma once

#include <GL/glew.h>
#include "ShaderManager.h"
#include "OpenGLLogger.h"
#include <iostream>
#include <fstream>
#include <sstream>

ShaderManager::ShaderManager()
{}

ShaderManager* ShaderManager::getSMInstance()
{
	static ShaderManager instance;
	return &instance;
}

ShaderProgramSource ShaderManager::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

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

unsigned int ShaderManager::CompileShader(unsigned int type, const std::string& source)
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

unsigned int ShaderManager::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLLog(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLLog(glAttachShader(program, vs));
	GLLog(glAttachShader(program, fs));
	GLLog(glLinkProgram(program));

	GLLog(glDetachShader(program, vs));
	GLLog(glDetachShader(program, fs));

	GLLog(glDeleteShader(vs));
	GLLog(glDeleteShader(fs));

	return program;
}