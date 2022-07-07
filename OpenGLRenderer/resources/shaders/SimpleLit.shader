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

#define MAX_DIRECTIONAL_LIGHTS 10
#define MAX_POINT_LIGHTS 50
#define MAX_SPOT_LIGHTS 50

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct Attenuation
{
	float Constant;
	float Linear;
	float Exponent;
};

struct BaseLight
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct PointLight
{
	BaseLight base;
	vec3 localPos;
	Attenuation atten;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float cutoff; // Cosine of angle (to reduce calculations for each pixel)
};

out vec4 fragColor;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_Normal;
in vec3 v_LocalPosition;

uniform Material u_Material;
uniform DirectionalLight u_DirLights[MAX_DIRECTIONAL_LIGHTS];
uniform int u_ActiveDirLightsCount;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform int u_ActivePointLightsCount;
uniform SpotLight u_SpotLights[MAX_SPOT_LIGHTS];
uniform int u_ActiveSpotLightsCount;
uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularExponent;
uniform vec3 u_CameraLocalPosition;

vec4 CalcBaseLightColor(in BaseLight light, in vec3 direction, in vec3 normal)
{
	vec4 ambientColor =
		vec4(light.color, 1.0f) *
		light.ambientIntensity *
		vec4(u_Material.ambientColor, 1.0f);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	float diffuseFactor = dot(normal, -direction);
	if (diffuseFactor > 0)
	{
		diffuseColor =
			vec4(light.color, 1.0f) *
			light.diffuseIntensity *
			vec4(u_Material.diffuseColor, 1.0f) *
			diffuseFactor;

		vec3 pixelToCamera = normalize(u_CameraLocalPosition - v_LocalPosition);	// Get Pixel to Camera Vector
		vec3 lightReflect = normalize(reflect(direction, normal));	// Get Reflection Vector
		float specularFactor = dot(pixelToCamera, lightReflect);					// Dot Product gives Specular Factor
		if (specularFactor > 0)
		{
			float specularExponent = texture2D(u_SpecularExponent, v_TexCoord).r * 255.0f;
			specularFactor = pow(specularFactor, specularExponent);
			specularColor =
				vec4(light.color, 1.0f) *
				light.specularIntensity *
				vec4(u_Material.specularColor, 1.0f) *
				specularFactor;
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcAllDirLightsColors(in vec3 normal)
{
	vec4 color = vec4(0, 0, 0, 1);

	for (int i = 0; i < u_ActiveDirLightsCount; i++)
	{
		color += CalcBaseLightColor(u_DirLights[i].base, u_DirLights[i].direction, normal);
	}

	return color;
}

vec4 CalcPointLightColor(in PointLight light, in vec3 normal)
{
	vec3 direction = v_LocalPosition - light.localPos;
	float distance = length(direction);
	direction = normalize(direction);

	float attenuation =
		light.atten.Constant +
		light.atten.Linear * distance +
		light.atten.Exponent * distance * distance;

	return CalcBaseLightColor(light.base, direction, normal) / attenuation;
}

vec4 CalcAllPointLightsColors(in vec3 normal)
{
	vec4 color = vec4(0, 0, 0, 0);

	for (int i = 0; i < u_ActivePointLightsCount; i++)
	{
		color += CalcPointLightColor(u_PointLights[i], normal);
	}

	return color;
}

vec4 CalcSpotLightColor(in SpotLight light, in vec3 normal)
{
	vec3 lightToPixel = normalize(v_LocalPosition - light.base.localPos);
	float spotFactor = dot(lightToPixel, light.direction);

	if (spotFactor > light.cutoff)
	{
		vec4 color = CalcPointLightColor(light.base, normal);
		float spotLightIntensity = 1.0f - (1.0f - spotFactor) / (1.0f - light.cutoff);
		return color * spotLightIntensity;
	}
	else
		return vec4(0, 0, 0, 0);
}

vec4 CalcAllSpotLightsColors(in vec3 normal)
{
	vec4 color = vec4(0, 0, 0, 0);

	for (int i = 0; i < u_ActiveSpotLightsCount; i++)
	{
		color += CalcSpotLightColor(u_SpotLights[i], normal);
	}

	return color;
}

void main()
{
	vec4 texColor = texture2D(u_DiffuseTexture, v_TexCoord);

	vec3 normal = normalize(v_Normal);
	vec4 TotalLightColor = CalcAllDirLightsColors(normal) + CalcAllPointLightsColors(normal) + CalcAllSpotLightsColors(normal);
	TotalLightColor = clamp(TotalLightColor, 0, 1);

	fragColor =
		texColor *
		TotalLightColor;
}