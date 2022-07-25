#include "Logger.h"
#include <GL/glew.h>
#include <cstdarg>

namespace core
{
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

	bool __Assert(bool expr, const char* expr_str, const char* file, int line, const std::string& msg)
	{
		if (!expr)
		{
			LogError(
				"Assert failed:\n" << msg << "\n"
				<< "Expected:\t" << expr_str << "\n"
				<< "Source:\t\t" << file << ", line " << line);
			return false;
		}
		return true;
	}
}