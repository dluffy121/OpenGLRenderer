#include "ShaderManager.h"
#include <Logger/Logger.h>

using namespace core;

ShaderManager::ShaderManager() {}

const unsigned int LOG_BUFF_SIZE = 1024;

ShaderManager* ShaderManager::getInstance()
{
	static ShaderManager instance;
	return &instance;
}

ShaderAsset& ShaderManager::CreateShaderAsset(const std::string& shaderPath)
{
	ShaderAsset* shaderAsset = nullptr;
	if (m_ShaderAssetCollection.find(shaderPath) != m_ShaderAssetCollection.end())
		shaderAsset = m_ShaderAssetCollection[shaderPath];
	else
	{
		shaderAsset = new ShaderAsset(shaderPath);
		m_ShaderAssetCollection[shaderPath] = shaderAsset;
	}

	return *shaderAsset;
}

ShaderAsset* ShaderManager::GetShaderAsset(const std::string& shaderPath)
{
	if (m_ShaderAssetCollection.find(shaderPath) == m_ShaderAssetCollection.end())
		return nullptr;

	return m_ShaderAssetCollection[shaderPath];
}

Shader* ShaderManager::LoadShader(ShaderAsset& shaderAsset)
{
	auto shaderPath = shaderAsset.GetPath();
	if (m_ShaderCache[shaderPath])
	{
		auto shader = m_ShaderCache[shaderPath];
		shader->m_RefCount++;
		return shader;
	}

	GLLog(unsigned int program = glCreateProgram());

	shaderAsset.Compile();

	GLLog(glAttachShader(program, shaderAsset.GetVertexShader()));
	GLLog(glAttachShader(program, shaderAsset.GetFragmentShader()));

	GLint status;

	GLLog(glLinkProgram(program));
	GLLog(glGetProgramiv(program, GL_LINK_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLsizei len;
		GLchar log[LOG_BUFF_SIZE];
		GLLog(glGetProgramInfoLog(program, LOG_BUFF_SIZE, &len, log));
		Log(log);
	}

	GLLog(glValidateProgram(program));
	GLLog(glGetProgramiv(program, GL_VALIDATE_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLsizei len;
		GLchar log[LOG_BUFF_SIZE];
		GLLog(glGetProgramInfoLog(program, LOG_BUFF_SIZE, &len, log));
		Log(log);
	}

	auto shader = new Shader(program, shaderPath);
	shader->m_RefCount++;
	m_ShaderCache[shaderPath] = shader;

	return shader;
}

Shader* ShaderManager::LoadShader(const std::string& shaderPath)
{
	if (m_ShaderCache[shaderPath])
	{
		auto shader = m_ShaderCache[shaderPath];
		shader->m_RefCount++;
		return shader;
	}

	ShaderAsset* shaderAsset = GetShaderAsset(shaderPath);
	ASSERT(shaderAsset, "Shader Asset does not exist for path: " + shaderPath);

	return LoadShader(*shaderAsset);
}

void ShaderManager::UnLoadShader(Shader* shader)
{
	if (--(shader->m_RefCount)) return;

	m_ShaderCache.erase(shader->Path);
	delete shader;
}

void ShaderManager::UnLoadShader(const std::string& shaderPath)
{
	if (!m_ShaderCache[shaderPath])
		return;

	UnLoadShader(m_ShaderCache[shaderPath]);
}
