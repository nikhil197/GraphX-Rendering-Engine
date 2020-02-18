#shader vertex
#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

out vec4 color;

void main()
{
	gl_Position = u_ProjectionView * u_Model * vPosition;
	color = vColor;
}

#shader fragment
#version 330 core

in vec4 vColor;

out vec4 fColor;

void main()
{
	fColor = vColor;
}