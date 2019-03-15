#shader vertex
#version 330 core

layout(location = 0) in vec2 vPosition;

// Uniforms
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(vPosition, 0.0f, 1.0f);
}

#shader fragment
#version 330 core

out vec4 fColor;

void main()
{
	fColor = vec4(1.0f);
}
