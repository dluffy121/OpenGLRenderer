#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 pixelPosition;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in float texIndex;
layout(location = 5) in vec3 normal;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out vec3 v_Normal;
out vec3 v_LocalPosition;

void main()
{
	gl_Position = pixelPosition;
	v_Color = color;
	v_TexCoord = texCoord;
	v_TexIndex = texIndex;
	v_Normal = normal;
	v_LocalPosition = position;
}

#shader fragment
#version 460 core

#define MAX_DIRECTIONAL_LIGHTS 50

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
	vec3 direction;
};

out vec4 fragColor;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_Normal;
in vec3 v_LocalPosition;

uniform Material u_Material;
uniform DirectionalLight u_DirLights[MAX_DIRECTIONAL_LIGHTS];
uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularExponent;
uniform vec3 u_CameraLocalPosition;

vec4 TotalAmbientColorFromDirLights()
{
	vec4 ambientColor = vec4(0, 0, 0, 0);

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		ambientColor +=
			vec4(u_DirLights[i].color, 1.0f) *
			u_DirLights[i].ambientIntensity;
	}

	return ambientColor;
}

void TotalDiffuseAndSpecularColorFromDirLights(
	in vec3 normal,
	out vec4 diffuseColor,
	out vec4 specularColor)
{
	diffuseColor = vec4(0, 0, 0, 0);
	specularColor = vec4(0, 0, 0, 0);

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		// Dot Product of Normal and light Direction gives Diffuse Factor
		// need to reverse Direction since we know the reflection vector is opposite of Light Vector and has same angle from normal
		float diffuseFactor = dot(normal, -u_DirLights[i].direction);
		if (diffuseFactor > 0)
		{
			diffuseColor +=
				vec4(u_DirLights[i].color, 1.0f) *
				u_DirLights[i].diffuseIntensity *
				diffuseFactor;

			vec3 pixelToCamera = normalize(u_CameraLocalPosition - v_LocalPosition);	// Get Pixel to Camera Vector
			vec3 lightReflect = normalize(reflect(u_DirLights[i].direction, normal));	// Get Reflection Vector
			float specularFactor = dot(pixelToCamera, lightReflect);					// Dot Product gives Specular Factor
			if (specularFactor > 0)
			{
				float specularExponent = texture2D(u_SpecularExponent, v_TexCoord).r * 255.0f;
				specularFactor = pow(specularFactor, specularExponent);
				specularColor +=
					vec4(u_DirLights[i].color, 1.0f) *
					u_DirLights[i].specularIntensity *
					specularFactor;
			}
		}
	}
}

void main()
{
	vec4 texColor = texture2D(u_DiffuseTexture, v_TexCoord);

	vec4 ambientColor =
		vec4(u_Material.ambientColor, 1.0f) +
		TotalAmbientColorFromDirLights();

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	TotalDiffuseAndSpecularColorFromDirLights(
		normalize(v_Normal),
		diffuseColor,
		specularColor);

	diffuseColor *= vec4(u_Material.diffuseColor, 1.0f);
	specularColor *= vec4(u_Material.specularColor, 1.0f);

	fragColor =
		texColor *
		clamp(ambientColor + diffuseColor + specularColor, 0.0f, 10.0f);
}