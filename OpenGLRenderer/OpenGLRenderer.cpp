#include "OpenGLHelper.cpp"
#include "ShaderManager.cpp"
#include <string>

OpenGLHelper* OpenGLHelper::instance = 0;
ShaderManager* ShaderManager::instance = 0;

int main(void)
{
	OpenGLHelper* OpenGLHelper = OpenGLHelper->getInstance();
	ShaderManager* ShaderManager = ShaderManager->getInstance();

	GLFWwindow* window;

	int retVal = OpenGLHelper->Init(window);
	if (retVal != int{})
		return retVal;

	float points[6]
	{
		-0.5, -0.5,
		 0.0,  0.5,
		 0.5, -0.5,
	};

	unsigned int bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 points;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = points;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(0.0, 0.0, 1.0, 1.0);\n"
		"}\n";

	unsigned int shader = ShaderManager->CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	OpenGLHelper->TerminateGLFW();
	return 0;
}