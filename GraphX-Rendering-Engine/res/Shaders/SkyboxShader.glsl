#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_TexCoord;

void main()
{
	v_TexCoord = vPosition;
	gl_Position = u_Projection * u_View * vec4(vPosition, 1.0);
}

#shader fragment
#version 330 core

uniform samplerCube u_Skybox;

in vec3 v_TexCoord;

uniform vec4 u_BlendColor;
uniform float u_BlendFactor;

out vec4 fColor;

void main()
{
	fColor = mix(texture(u_Skybox, v_TexCoord), u_BlendColor, u_BlendFactor);
}