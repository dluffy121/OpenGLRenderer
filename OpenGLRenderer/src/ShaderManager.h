#pragma once

#include <string>

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

class ShaderManager
{
public:
	static ShaderManager* getSMInstance(void);

	ShaderProgramSource ParseShader(const std::string&);
	unsigned int CreateShader(const std::string&, const std::string&);

protected:
	ShaderManager();

private:
	ShaderManager(const ShaderManager&) = delete;								// To avoid Cloning
	ShaderManager& operator=(const ShaderManager&) = delete;					// To avoid Assignment

	unsigned int CompileShader(unsigned int, const std::string&);
};