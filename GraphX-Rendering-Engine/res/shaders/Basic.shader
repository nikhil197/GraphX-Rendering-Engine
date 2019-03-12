#shader vertex
# version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(position, 1.0f);
	TexCoords = texCoord;
}

#shader fragment
#version 330 core

in vec2 TexCoords;

uniform vec4 u_Color;

uniform sampler2D u_Tex;

out vec4 color;

void main()
{
	float depthValue = texture(u_Tex, TexCoords).r;
	color = vec4(vec3(depthValue), 1.0);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}