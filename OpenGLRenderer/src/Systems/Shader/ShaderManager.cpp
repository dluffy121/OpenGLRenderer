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
	if (shaderAssetCollection.find(shaderPath) != shaderAssetCollection.end())
		shaderAsset = shaderAssetCollection[shaderPath];
	else
	{
		shaderAsset = new ShaderAsset(shaderPath);
		shaderAssetCollection[shaderPath] = shaderAsset;
	}

	return *shaderAsset;
}

ShaderAsset* ShaderManager::GetShaderAsset(const std::string& shaderPath)
{
	if (shaderAssetCollection.find(shaderPath) == shaderAssetCollection.end())
		return nullptr;

	return shaderAssetCollection[shaderPath];
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