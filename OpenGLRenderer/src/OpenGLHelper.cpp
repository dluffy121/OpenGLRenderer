#pragma once
#include <GL/glew.h>
#include <cstdio>
#include <GLFW/glfw3.h>

class OpenGLHelper
{
	private:
	static OpenGLHelper* instance;

	public:
	static OpenGLHelper* getInstance() {
		if (!instance)
			instance = new OpenGLHelper;
		return instance;
	}

	public:
	int Init(GLFWwindow*& window)
	{
		if (!InitializeGLFW())
			return -1;

		window = CreateWindow();
		if (!window)
		{
			TerminateGLFW();
			return -1;
		}

		MakeWindow(window);

		if (!InitializeGLEW())
			return -1;

		fprintf(stdout, "Status: Using GL %s\n", glGetString(GL_VERSION));

		return 0;
	}

	private:
	int InitializeGLFW()
	{
		int retVal = glfwInit();
		fprintf(stdout, "Status: Using GLFW %s\n", glfwGetVersionString());
		return retVal;
	}

	public:
	void TerminateGLFW()
	{
		glfwTerminate();
	}

	private:
	bool InitializeGLEW()
	{
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			return false;
		}
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		return true;
	}

	private:
	GLFWwindow* CreateWindow()
	{
		return glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	}

	private:
	void MakeWindow(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);
	}
};

