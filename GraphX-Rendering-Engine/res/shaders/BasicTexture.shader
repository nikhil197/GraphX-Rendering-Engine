#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

// uniforms
uniform mat4 u_Model;
uniform mat4 u_ProjectionView;
uniform mat3 u_Normal;

// varying variables
out vec2 v_TexCoord;
out vec3 v_WorldPosition;
out vec3 v_Normal;

void main()
{
	v_WorldPosition = vec3(u_Model * vec4(vPosition, 1.0));
	gl_Position = u_ProjectionView * vec4(v_WorldPosition, 1.0);
	v_TexCoord = vTexCoord;
	v_Normal = u_Normal * vNormal;
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;
in vec3 v_WorldPosition;
in vec3 v_Normal;

//uniforms
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

uniform float u_AmbientStrength;
uniform float u_Shininess;
uniform float u_Reflectivity;

// Sampler uniform to sample from the texture
uniform sampler2D u_Texture;

out vec4 fColor;

void main()
{
	vec3 UnitNormal = normalize(v_Normal);
	
	// Ambient Light Color
	vec4 AmbientColor = u_AmbientStrength * u_LightColor;

	// Diffuse Light Color
	vec3 UnitLightVec = normalize(u_LightPos - v_WorldPosition);
	float brightness = max(dot(UnitLightVec, UnitNormal), 0.0);
	vec4 DiffuseColor = brightness * u_LightColor;

	// Specular Light Color
	vec3 ViewVec = normalize(u_CameraPos - v_WorldPosition);
	vec3 ReflectedLightVec = reflect(-UnitLightVec, UnitNormal);
	float shine = pow(max(dot(ReflectedLightVec, ViewVec), 0.0), u_Shininess);
	vec4 SpecularColor = u_Reflectivity * shine * u_LightColor;

	// Texture Color
	vec4 texColor = texture(u_Texture, v_TexCoord);
	fColor = (AmbientColor + DiffuseColor + SpecularColor) * texColor;
}