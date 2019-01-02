#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 u_View;
uniform mat4 u_Projection;

out v_TexCoord;

void main()
{
	v_TexCoord = vPosition;
	gl_Position = u_Projection * u_View * vec4(vPosition, 1.0);
}

#shader fragment
#version 330 core

uniform samplerCube skybox;

in v_TexCoord;

out fColor;

void main()
{
	fColor = texture(skybox, v_TexCoord);
}