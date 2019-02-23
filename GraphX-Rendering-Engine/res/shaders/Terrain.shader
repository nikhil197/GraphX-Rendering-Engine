#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

/* Uniforms */
uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Projection = mat4(1.0f);

/* Data sent to the fragment shader */
out struct Data
{
	vec3 Normal;
	vec2 TexCoord;
	vec3 WorldPosition;
} v_Data;

void main()
{
	v_Data.WorldPosition = vec3(u_Model * vec4(vPosition, 1.0f));
	gl_Position = u_Projection * u_View * vec4(v_Data.WorldPosition, 1.0f);
	v_Data.Normal = vNormal;
	v_Data.TexCoord = vTexCoord;
}

#shader fragment
#version 330 core

/* Data Recieved from the vertex shader */
in struct Data
{
	vec3 Normal;
	vec2 TexCoord;
	vec3 WorldPosition;
} v_Data;

uniform sampler2D u_Texture0;

uniform vec4 u_Color = vec4(1.0f, 0.0f, 0.0f, 0.0f);

/* Final color */
out vec4 fColor;

void main()
{
	vec4 texColor = texture(u_Texture0, v_Data.TexCoord);
	fColor = texColor + u_Color;
}
