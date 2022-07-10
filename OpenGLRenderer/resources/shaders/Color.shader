#shader vertex
#version 460 core

layout(location = 1) in vec4 pixelPosition;
layout(location = 2) in vec4 color;

out vec4 v_Color;

void main()
{
	gl_Position = pixelPosition;
	v_Color = color;
}

#shader fragment
#version 460 core

out vec4 fragColor;

in vec4 v_Color;

void main()
{
	fragColor = v_Color;
}