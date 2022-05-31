#pragma once

#include <string>
#include <vector>
#include <Component/Component.h>

namespace scene
{
	class Scene
	{
	private:
		friend class HeirarchyWindow;

	private:
		bool m_Active;
		std::string m_Name;
		std::vector<GameVastu*> m_GameVastus;

	public:
		Scene(std::string name);
		virtual ~Scene();

		inline bool& IsActive() { return m_Active; }
		inline std::string GetName() const { return m_Name; }

		GameVastu* CreateGameVastu();
		void DestroyGameVastu(GameVastu*& gameVastu);

		virtual void Update() {}
	};
}

template<class T>
concept Scene_T = requires
{
	std::is_base_of<scene::Scene, T>::value == true && std::is_pointer<T>::value == true;
};

template<class T>
concept Scene_T_ptr = requires
{
	std::is_base_of<scene::Scene*, T*>::value == true && std::is_pointer<T*>::value == true;
};