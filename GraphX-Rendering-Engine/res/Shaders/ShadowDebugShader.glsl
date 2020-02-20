#shader vertex
# version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;

out vec2 v_TexCoords;

uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_ProjectionView = mat4(1.0f);

void main()
{
	gl_Position = u_ProjectionView * u_Model * vec4(vPosition, 1.0f);
	v_TexCoords = vTexCoord;
}

#shader fragment
#version 330 core

in vec2 v_TexCoords;

uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	float depthValue = texture(u_Texture, v_TexCoords).r;
	//color = vec4(vec3(depthValue), 1.0);
	color = vec4(v_TexCoords, 0.0f, 1.0f);
}