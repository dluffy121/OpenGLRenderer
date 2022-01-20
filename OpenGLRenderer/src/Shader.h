#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

enum class ShaderType
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

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_ShaderId;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void UnBind() const;

	inline const std::string& GetFilePath() { return m_FilePath; }
	inline unsigned int GetShaderId() { return m_ShaderId; }

	// Set Uniform
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSource ParseShader();
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	GLint GetUniformLocation(const std::string& name);
};