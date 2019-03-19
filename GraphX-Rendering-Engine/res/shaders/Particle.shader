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
out vec4 fColor;

void main()
{
	fColor = texture(u_ParticleTexture, v_TexCoords);
}
