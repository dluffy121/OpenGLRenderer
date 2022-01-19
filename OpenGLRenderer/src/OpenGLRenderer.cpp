#include "OpenGLLogger.h"
#include "OpenGLHelper.h"
#include "ShaderManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

int main(void)
{
	OpenGLHelper* openGLHelperRef = OpenGLHelper::getOGHInstance();
	ShaderManager* shaderManagerRef = ShaderManager::getSMInstance();

	GLFWwindow* window = nullptr;

	int retVal = openGLHelperRef->Init(window);
	if (retVal != int{})
		return retVal;

	{
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

		VertexArray va;
		VertexBuffer vb(points, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		ShaderProgramSource source = shaderManagerRef->ParseShader("resources/shaders/Basic.shader");
		unsigned int shader = shaderManagerRef->CreateShader(source.VertexSource, source.FragmentSource);
		GLLog(glUseProgram(shader));

		GLLog(GLuint uniformId = glGetUniformLocation(shader, "u_Color"));
		ASSERT(uniformId != -1);
		GLLog(glUniform4f(uniformId, 0.2f, 0.3f, 0.8f, 1.0f));

		va.UnBind();
		GLLog(glUseProgram(0));
		GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLLog(glBindBuffer(GL_ARRAY_BUFFER, 0));

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLLog(glClear(GL_COLOR_BUFFER_BIT));

			GLLog(glUseProgram(shader));
			GLLog(glUniform4f(uniformId, r, 0.3f, 0.8f, 1.0f));

			va.Bind();
			//GLLog(glBindVertexArray(vao));
			ib.Bind();
			//GLLog(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

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
	}

	openGLHelperRef->TerminateGLFW();
	return 0;
}