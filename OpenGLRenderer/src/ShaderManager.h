#pragma once
#include <unordered_map>
#include "ShaderAsset.h"

class ShaderManager
{
private:
	std::unordered_map<std::string, ShaderAsset*> shaderAssetCollection;
	//std::unordered_map<unsigned int, Shader*> shaderCollection;

public:
	static ShaderManager* getInstance();

	ShaderAsset& CreateShaderAsset(const std::string& filename);
	ShaderAsset* GetShaderAsset(const std::string& shaderPath);
	unsigned int LoadShader(ShaderAsset& shaderAsset);
	unsigned int LoadShader(const std::string& shaderPath);

protected:
	ShaderManager();

private:
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager operator=(const ShaderManager&) = delete;
};