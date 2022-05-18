#shader vertex
#version 460 core

layout(location = 0) in vec4 points;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = points;
	v_TexCoord = texCoord;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}