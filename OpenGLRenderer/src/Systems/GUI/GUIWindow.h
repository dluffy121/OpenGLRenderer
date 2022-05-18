#pragma once

#include <string>

class GUIWindow
{
protected:
	bool m_Active;
	std::string m_Name;

private:
	GUIWindow(std::string Name) :
		m_Name(Name),
		m_Active(true)
	{}

public:
	virtual void Draw() const = 0;
};