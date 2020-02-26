#shader vertex
#version 330 core

layout(location = 0) in vec2 vPosition;

out vec2 v_TexCoords;

// Uniforms
uniform mat4 u_Model;
uniform mat4 u_Projection;

void main()
{
	v_TexCoords = vPosition + vec2(0.5f, 0.5f);
	gl_Position = u_Projection * u_Model * vec4(vPosition, 0.0f, 1.0f);
}

#shader fragment
#version 330 core

in vec2 v_TexCoords;

uniform sampler2D u_ParticleTexture;
uniform vec4 u_TexCoordOffsets = vec4(0.0f);
uniform int u_TexAtlasRows = 0;
uniform float u_BlendFactor = 0.0f;
uniform vec4 u_Color = vec4(1.0f);

out vec4 fColor;

void main()
{
	// Means texture is not used
	if (u_TexAtlasRows == 0 || u_TexAtlasRows == 1)
	{
		fColor = texture(u_ParticleTexture, v_TexCoords) * u_Color;
	}
	else
	{
		vec2 TexCoords1 = (v_TexCoords / u_TexAtlasRows) + u_TexCoordOffsets.xy;
		vec2 TexCoords2 = (v_TexCoords / u_TexAtlasRows) + u_TexCoordOffsets.zw;
		fColor = mix(texture(u_ParticleTexture, TexCoords1), texture(u_ParticleTexture, TexCoords2), u_BlendFactor);
	}
}
