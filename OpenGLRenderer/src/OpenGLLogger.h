#pragma once

#define ASSERT(x)	if(!(x)) __debugbreak();
#define GLLog(x)	GLClearError();\
					x;\
					ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(void);
bool GLLogCall(const char*, const char*, int);
