#pragma once

#include <GUI/GUIWindow.h>

class HeirarchyWindow : public GUIWindow
{
public:
	HeirarchyWindow();
	~HeirarchyWindow();

	void Draw(Window* window) override;
};