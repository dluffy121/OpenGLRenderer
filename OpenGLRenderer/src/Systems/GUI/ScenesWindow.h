#pragma once

#include "GUIWindow.h"
#include <unordered_map>
#include <functional>
#include "../Scene/Scene.h"

class ScenesWindow : public GUIWindow
{
private:
	std::unordered_map<std::string, std::function<scene::Scene* ()>> m_SceneToggles;

	const char* loadString = "Load";
	const char* unloadString = "UnLoad";

public:
	ScenesWindow();
	~ScenesWindow();

	void Draw(Window* window) override;

	template<Scene_T T>
	void RegisterScene(const std::string& name)
	{
		if (m_SceneToggles.find(name) != m_SceneToggles.end())
			return;

		m_SceneToggles[name] = []() { return new T(); };
	}

	template<Scene_T T>
	void UnRegisterScene(const std::string& name)
	{
		if (m_SceneToggles.find(name) == m_SceneToggles.end())
			return;

		m_SceneToggles.erase(name);
	}
};