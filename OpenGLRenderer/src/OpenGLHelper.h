#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OpenGLHelper
{
public:
	static OpenGLHelper* getOGHInstance();

	int Init(GLFWwindow*&);
	void TerminateGLFW();

protected:
	OpenGLHelper();

private:
	OpenGLHelper(const OpenGLHelper&) = delete;					// To avoid Cloning
	OpenGLHelper& operator=(const OpenGLHelper&) = delete;		// To avoid Assignment

	int InitializeGLFW();
	bool InitializeGLEW();
	GLFWwindow* CreateWindow();
	void MakeWindow(GLFWwindow*);
};