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
	GLLog(glGenBuffers(1, &bufferId));															// Generate 1 Buffer and get its id in 'bufferId' https://docs.gl/gl4/glGenBuffers
	GLLog(glBindBuffer(GL_ARRAY_BUFFER, bufferId));												// Bind the generated buffer with its id https://docs.gl/gl4/glBindBuffer
	GLLog(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), points, GL_STATIC_DRAW));		// update buffer data with its type of usage https://docs.gl/gl4/glBufferData

	GLLog(glEnableVertexAttribArray(0));														// enables vertex atrrib array from the bound buffer https://docs.gl/gl4/glEnableVertexAttribArray
	GLLog(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));				// define an array vertex attribute data

	unsigned int ibo;
	GLLog(glGenBuffers(1, &ibo));
	GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLLog(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	ShaderProgramSource source = ShaderManager->ParseShader("resources/shaders/Basic.shader");
	unsigned int shader = ShaderManager->CreateShader(source.VertexSource, source.FragmentSource);
	GLLog(glUseProgram(shader));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLLog(glClear(GL_COLOR_BUFFER_BIT));

		//OpenGLHelper->GLClearError();
		//glDrawArrays(GL_TRIANGLES, 0, 6);														// this method will draw from binded buffer array https://docs.gl/gl4/glDrawArrays
		GLLog(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));								// this method will draw from binded element buffer array https://docs.gl/gl4/glDrawElements
		//ASSERT(OpenGLHelper->GLLogCall());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLLog(glDeleteProgram(shader));

	OpenGLHelper->TerminateGLFW();
	return 0;
}