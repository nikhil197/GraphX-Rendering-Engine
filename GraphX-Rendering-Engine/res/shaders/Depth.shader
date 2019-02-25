#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 u_LightSpaceMatrix = mat4(1.0f);
uniform mat u_Model = mat4(1.0f);

void main()
{
	gl_Position = u_LightSpaceMatrix * u_Model * vec4(vPosition, 1.0f);
}

#shader fragment
#version 330 core

void main()
{
	// No need to do anything, since the depth values are directly being written to the depth map
}