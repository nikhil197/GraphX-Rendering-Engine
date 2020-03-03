#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoords;
layout(location = 3) in float vTexIndex;

uniform mat4 u_ProjectionView = mat4(1.0f);

out vec2 v_TexCoords;
out vec4 v_Color;
out float v_TexIndex;

void main()
{
	gl_Position = u_ProjectionView * vec4(vPosition, 1.0f);
	v_TexCoords = vTexCoords;
	v_Color = vColor;
	v_TexIndex = vTexIndex;
}

#shader fragment

#version 330 core
in vec2 v_TexCoords;
in vec4 v_Color;
in float v_TexIndex;

uniform float u_Tiling = 1.0f;
uniform sampler2D u_Textures[32];

out vec4 color;

void main()
{
	int index = int(ceil(v_TexIndex));
	color = texture(u_Textures[index], v_TexCoords * u_Tiling) * v_Color;
}