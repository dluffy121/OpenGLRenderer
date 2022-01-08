#include <GL/glew.h>
#include "OpenGLLogger.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
	//while (!glGetError());		// since GL_NO_ERROR is equals to 0
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())	// this while loop will not end until 'error' is 0
	{
		std::cout << "\n"
			"[OpenGL error]: (" << error << ")" << "\n"
			"  Function: " << function << "\n"
			"  File: " << file << "\n"
			"  Line: " << line << std::endl;
		return false;
	}
	return true;
}