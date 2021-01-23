#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoords1;
layout(location = 3) in vec2 vTexCoords2;
layout(location = 4) in float vTexAtlasRows;
layout(location = 5) in float vBlendFactor;
layout(location = 6) in float vTexIndex;

out vec4 v_Color;
out vec2 v_TexCoords1;
out vec2 v_TexCoords2;
out float v_TexAtlasRows;
out float v_BlendFactor;
out float v_TexIndex;

// Uniforms
uniform mat4 u_Projection;

void main()
{
	v_Color = vColor;
	v_TexCoords1 = vTexCoords1;
	v_TexCoords2 = vTexCoords2;
	v_TexAtlasRows = vTexAtlasRows;
	v_BlendFactor = vBlendFactor;
	v_TexIndex = vTexIndex;
	gl_Position = u_Projection * vec4(vPosition, 1.0f);
}

#shader fragment
#version 330 core

in vec4 v_Color;
in vec2 v_TexCoords1;
in vec2 v_TexCoords2;
in float v_TexAtlasRows;
in float v_BlendFactor;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

out vec4 fColor;

void main()
{
	int index = int(ceil(v_TexIndex));

	// Means texture atlas is not used
	if (v_TexAtlasRows == 0 || v_TexAtlasRows == 1)
	{
		fColor = texture(u_Textures[index], v_TexCoords1) * v_Color;
	}
	else
	{
		fColor = mix(texture(u_Textures[index], v_TexCoords1), texture(u_Textures[index], v_TexCoords2), v_BlendFactor) * v_Color;
	}
}
