#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

class Shader
{
private:
	std::string filePath;
	unsigned int shaderId;
	std::unordered_map<std::string, int> uniformLocationCache;

public:
	Shader(unsigned int shaderId);
	~Shader();

	void Bind() const;
	void UnBind() const;

	inline const std::string& GetFilePath() { return filePath; }
	inline unsigned int GetShaderId() { return shaderId; }

	// Set Uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	GLint GetUniformLocation(const std::string& name);
};