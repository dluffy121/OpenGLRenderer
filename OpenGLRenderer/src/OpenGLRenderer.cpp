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
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	ShaderProgramSource source = ShaderManager->ParseShader("resources/shaders/Basic.shader");
	unsigned int shader = ShaderManager->CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	OpenGLHelper->TerminateGLFW();
	return 0;
}