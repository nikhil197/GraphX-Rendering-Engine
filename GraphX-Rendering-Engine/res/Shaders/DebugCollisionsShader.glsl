#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model = mat4(1.0f);

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(vPosition, 1.0f);
}

#shader fragment
#version 330 core

uniform vec4 u_DebugColor;

out vec4 fColor;

void main()
{
	fColor = u_DebugColor;
}