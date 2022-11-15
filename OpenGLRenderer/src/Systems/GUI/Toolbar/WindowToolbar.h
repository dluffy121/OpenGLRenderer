#pragma once

#include "GUI/GUIWindow.h"

class WindowToolbar : public GUIWindow
{
public:
	WindowToolbar();
	~WindowToolbar();

	void Draw(Window* window) override;
};