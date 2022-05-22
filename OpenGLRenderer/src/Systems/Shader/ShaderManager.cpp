#include "ShaderManager.h"
#include "Shader.h"
#include "../../Core/Logger.h"

ShaderManager::ShaderManager() {}

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

unsigned int ShaderManager::LoadShader(ShaderAsset& shaderAsset)
{
	GLLog(unsigned int program = glCreateProgram());

	shaderAsset.Compile();

	GLLog(glAttachShader(program, shaderAsset.GetVertexShader()));
	GLLog(glAttachShader(program, shaderAsset.GetFragmentShader()));
	GLLog(glLinkProgram(program));
	GLLog(glValidateProgram(program));

	return program;
}

unsigned int ShaderManager::LoadShader(const std::string& shaderPath)
{
	ShaderAsset* shaderAsset = GetShaderAsset(shaderPath);
	if (shaderAsset == nullptr)
		return 0;

	return LoadShader(*shaderAsset);
}