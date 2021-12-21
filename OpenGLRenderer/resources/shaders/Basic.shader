#shader vertex
#version 330 core

layout(location = 0) in vec4 points;

void main()
{
	gl_Position = points;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.0, 0.0, 1.0, 1.0);
}