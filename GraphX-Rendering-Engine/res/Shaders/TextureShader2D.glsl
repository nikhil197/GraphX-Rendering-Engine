#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoords;

uniform mat4 u_ProjectionView = mat4(1.0f);
uniform mat4 u_Model = mat4(1.0f);

out vec2 v_TexCoords;

void main()
{
	gl_Position = u_ProjectionView * u_Model * vec4(vPosition, 1.0f);
	v_TexCoords = vTexCoords;
}

#shader fragment

#version 330 core
in vec2 v_TexCoords;

uniform float u_Tiling = 1.0f;
uniform vec4 u_Tint;
uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	color = texture(u_Texture, v_TexCoords * u_Tiling) * u_Tint;
}