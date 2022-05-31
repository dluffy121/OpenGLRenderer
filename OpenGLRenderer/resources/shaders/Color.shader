#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

void main()
{
	gl_Position = vec4(position, 1.0);
	v_Color = color;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}