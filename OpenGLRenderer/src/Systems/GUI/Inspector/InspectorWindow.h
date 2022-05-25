#pragma once

#include "../GUIWindow.h"

class InspectorWindow : public GUIWindow
{
public:
	InspectorWindow();
	~InspectorWindow();

	void Draw(Window* window) override;
};