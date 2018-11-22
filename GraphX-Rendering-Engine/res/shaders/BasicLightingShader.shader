#shader vertex
#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec4 vColor;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 vPos;
out vec3 vNml;
out vec4 color;
void main() {
	vec4 worldPosition = u_Model * vPosition;
	vPos = worldPosition.xyz;
	vNml = (u_Model * vNormal).xyz;
	gl_Position = u_Projection * u_View * worldPosition;
	color = vColor;
}

#shader fragment
#version 330 core

in vec4 color;
in vec3 vPos;
in vec3 vNml;

out vec4 fColor;

void main()
{
	fColor = color;
}