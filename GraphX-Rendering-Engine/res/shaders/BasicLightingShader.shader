#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColor;

// Uniform matrices for the transformations
uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat3 u_Normal;

// Uniforms for the light
uniform vec3 u_LightPos;

out vec3 p_Normal;
out vec3 p_LightVec;	// Vector from the world position to the light position
out vec4 p_Color;

void main()
{
	vec3 worldPosition = vec3(u_Model * vec4(vPosition, 1.0f));
	gl_Position = u_Projection * u_View * vec4(worldPosition, 1.0f);
	p_Normal = u_Normal * vNormal;
	p_Color = vColor;
	p_LightVec = u_LightPos - worldPosition;
}

#shader fragment
#version 330 core

in vec4 p_Color;
in vec4 p_Normal;
in vec4 p_LightVec;	

// Uniforms
uniform vec4 u_LightColor;

out vec4 fColor;

void main()
{
	// Normalize the light vector and the normal vector
	vec4 UnitNormal = normalize(p_Normal);
	vec4 UnitLightVec = normalize(p_LightVec);

	float brightness = max(0.0f, dot(UnitNormal, UnitLightVec));
	vec4 diffuseColor = u_LightColor * brightness;

	fColor = diffuseColor * p_Color;
}