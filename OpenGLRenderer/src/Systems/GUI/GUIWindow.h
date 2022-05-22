#pragma once

#include <string>
#include "imgui/imgui.h"

class Window;

class GUIWindow
{
protected:
	bool m_Active;
	std::string m_Name;

protected:
	GUIWindow(const std::string& Name) :
		m_Name(Name),
		m_Active(true)
	{}

public:
	virtual void Draw(Window* window) = 0;
};