#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * vec4(vPosition, 1.0f);
}

#shader fragment
#version 330 core

uniform vec4 u_DebugColor;

out vec4 fColor;

void main()
{
	fColor = u_DebugColor;
}