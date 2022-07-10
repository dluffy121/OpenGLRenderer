#shader vertex
#version 460 core

layout(location = 1) in vec4 pixelPosition;
layout(location = 3) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = pixelPosition;
	v_TexCoord = texCoord;
}

#shader fragment
#version 460 core

struct Material
{
	vec3 diffuseColor;
};

out vec4 fragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;	// 0

void main()
{
	vec4 texColor = texture2D(u_Texture, v_TexCoord);
	fragColor = texColor;
}