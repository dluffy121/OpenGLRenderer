#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGLLogger.h"
#include "OpenGLHelper.h"
#include "ShaderManager.h"

int main(void)
{
	OpenGLHelper* openGLHelperRef = OpenGLHelper::getOGHInstance();
	ShaderManager* shaderManagerRef = ShaderManager::getSMInstance();

	GLFWwindow* window = nullptr;

	int retVal = openGLHelperRef->Init(window);
	if (retVal != int{})
		return retVal;

	float points[]
	{
		-0.5, -0.5, //0
		 0.5, -0.5, //1
		 0.5,  0.5, //2
		-0.5,  0.5  //3
	};

	unsigned int indices[]
	{
		0,1,2,
		2,3,0
	};

	unsigned int bufferId;
	GLLog(glGenBuffers(1, &bufferId));															// Generate 1 Buffer and get its id in 'bufferId' https://docs.gl/gl4/glGenBuffers
	GLLog(glBindBuffer(GL_ARRAY_BUFFER, bufferId));												// Bind the generated buffer with its id https://docs.gl/gl4/glBindBuffer
	GLLog(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), points, GL_STATIC_DRAW));		// update buffer data with its type of usage https://docs.gl/gl4/glBufferData

	GLLog(glEnableVertexAttribArray(0));														// enables vertex atrrib array from the bound buffer https://docs.gl/gl4/glEnableVertexAttribArray
	GLLog(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));				// define an array vertex attribute data

	unsigned int ibo;
	GLLog(glGenBuffers(1, &ibo));
	GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLLog(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	ShaderProgramSource source = shaderManagerRef->ParseShader("resources/shaders/Basic.shader");
	unsigned int shader = shaderManagerRef->CreateShader(source.VertexSource, source.FragmentSource);
	GLLog(glUseProgram(shader));

	GLLog(GLuint uniformId = glGetUniformLocation(shader, "u_Color"));
	ASSERT(uniformId != -1);
	GLLog(glUniform4f(uniformId, 0.2f, 0.3f, 0.8f, 1.0f));

	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLLog(glClear(GL_COLOR_BUFFER_BIT));

		GLLog(glUniform4f(uniformId, r, 0.3f, 0.8f, 1.0f));

		//OpenGLHelper->GLClearError();
		//glDrawArrays(GL_TRIANGLES, 0, 6);														// this method will draw from binded buffer array https://docs.gl/gl4/glDrawArrays
		GLLog(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));						// this method will draw from binded element buffer array https://docs.gl/gl4/glDrawElements
		//ASSERT(OpenGLHelper->GLLogCall());

		if (r > 1)
			increment = -0.05f;
		else if (r < 0)
			increment = 0.05f;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLLog(glDeleteProgram(shader));

	//openGLHelperRef->TerminateGLFW();
	return 0;
}