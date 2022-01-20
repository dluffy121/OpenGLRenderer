#include "Shader.h"
#include "OpenGLLogger.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filename) :
	m_FilePath(filename),
	m_ShaderId(0)
{
	ShaderProgramSource source = ParseShader();
	m_ShaderId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLLog(glDeleteProgram(m_ShaderId));
}

void Shader::Bind() const
{
	GLLog(glUseProgram(m_ShaderId));
}

void Shader::UnBind() const
{
	GLLog(glUseProgram(0));
}

ShaderProgramSource Shader::ParseShader()
{
	std::ifstream stream(m_FilePath);

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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLLog(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLLog(GLint location = glGetUniformLocation(m_ShaderId, name.c_str()));
	m_UniformLocationCache[name] = location;
	return location;
}
