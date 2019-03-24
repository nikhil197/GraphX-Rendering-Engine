#shader vertex
#version 330 core

layout(location = 0) in vec2 vPosition;

out vec2 v_TexCoords;

// Uniforms
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	v_TexCoords = vPosition + vec2(0.5f, 0.5f);
	gl_Position = u_Projection * u_View * u_Model * vec4(vPosition, 0.0f, 1.0f);
}

#shader fragment
#version 330 core

in vec2 v_TexCoords;

uniform sampler2D u_ParticleTexture;
uniform vec2 u_TexCoordOffset1;
uniform vec2 u_TexCoordOffset2;
uniform int u_TexAtlasRows;
uniform float u_BlendFactor;

out vec4 fColor;

void main()
{
	vec2 TexCoords1 = (v_TexCoords / u_TexAtlasRows) + u_TexCoordOffset1;
	vec2 TexCoords2 = (v_TexCoords / u_TexAtlasRows) + u_TexCoordOffset2;
	fColor = mix(texture(u_ParticleTexture, TexCoords1), texture(u_ParticleTexture, TexCoords2), u_BlendFactor);
}
