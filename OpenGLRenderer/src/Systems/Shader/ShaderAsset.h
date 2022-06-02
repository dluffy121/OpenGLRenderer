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
	const std::string m_Path;
	ShaderProgramSource m_ProgramSource;
	unsigned int m_VertexShader;
	unsigned int m_FragmentShader;

public:
	ShaderAsset(const std::string& shaderPath);
	~ShaderAsset();

	void Compile();

	inline const std::string& GetPath() const { return m_Path; }
	inline unsigned int GetVertexShader() const { return m_VertexShader; }
	inline unsigned int GetFragmentShader() const { return m_FragmentShader; }

private:
	ShaderProgramSource ParseShader();
	unsigned int CompileShader(unsigned int type, const std::string& source);
};