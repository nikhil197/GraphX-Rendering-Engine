#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_LightSpaceMatrix * u_Model * vec4(vPosition, 1.0f);
}

#shader fragment
#version 330 core

void main()
{
	// No need to do anything, since the depth values are directly being written to the depth map
	// gl_FragDepth = gl_FragCoord.z
}