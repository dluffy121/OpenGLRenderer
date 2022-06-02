#pragma once
#include <unordered_map>
#include <Shader/Shader.h>
#include "ShaderAsset.h"

class ShaderManager
{
private:
	std::unordered_map<std::string, ShaderAsset*> m_ShaderAssetCollection;
	std::unordered_map<std::string, Shader*> m_ShaderCache;

public:
	static ShaderManager* getInstance();

	ShaderAsset& CreateShaderAsset(const std::string& filename);
	ShaderAsset* GetShaderAsset(const std::string& shaderPath);
	Shader* LoadShader(ShaderAsset& shaderAsset);
	Shader* LoadShader(const std::string& shaderPath);
	void UnLoadShader(Shader* shader);
	void UnLoadShader(const std::string& shaderPath);

protected:
	ShaderManager();

private:
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager operator=(const ShaderManager&) = delete;
};