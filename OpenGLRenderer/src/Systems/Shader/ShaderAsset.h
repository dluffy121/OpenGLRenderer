#pragma once
#include <string>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

enum class ShaderType
{
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1
};

class ShaderAsset
{
private:
	std::string shaderPath;
	ShaderProgramSource programSource;
	unsigned int VertexShader;
	unsigned int FragmentShader;

public:
	ShaderAsset(const std::string& shaderPath);
	~ShaderAsset();

	void Compile();

	inline unsigned int GetVertexShader() const { return VertexShader; }
	inline unsigned int GetFragmentShader() const { return FragmentShader; }

private:
	ShaderProgramSource ParseShader();
	unsigned int CompileShader(unsigned int type, const std::string& source);
};