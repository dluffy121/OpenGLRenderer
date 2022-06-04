#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
	gl_Position = position;
	v_Color = color;
	v_TexCoord = texCoord;
	v_TexIndex = texIndex;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{
	int index = int(v_TexIndex) - 1;
	if (index < 0)
	{
		color = v_Color;
	}
	else
	{
		vec4 texColor = texture(u_Textures[index], v_TexCoord);
		color = texColor * v_Color;
	}
}