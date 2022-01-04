#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <sstream>
#include "OpenGLHelper.cpp"

enum ShaderType
{
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1
};

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class ShaderManager
{
	private:
	static ShaderManager* instance;

	public:
	static ShaderManager* getInstance() {
		if (!instance)
			instance = new ShaderManager;
		return instance;
	}

	public:
	ShaderProgramSource ParseShader(const std::string& filepath)
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

	private:
	unsigned int CompileShader(unsigned int type, const std::string& source)
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

	public:
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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
};