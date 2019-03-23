#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

/* Uniforms */
uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Projection = mat4(1.0f);
uniform mat4 u_LightSpaceMatrix = mat4(1.0f);

/* Data sent to the fragment shader */
out struct Data
{
	vec3 Normal;
	vec2 TexCoord;
	vec3 WorldPosition;
	vec4 LightSpacePos;
} v_Data;

void main()
{
	v_Data.WorldPosition = vec3(u_Model * vec4(vPosition, 1.0f));
	gl_Position = u_Projection * u_View * vec4(v_Data.WorldPosition, 1.0f);
	v_Data.Normal = vNormal;
	v_Data.TexCoord = vTexCoord;
	v_Data.LightSpacePos = u_LightSpaceMatrix * vec4(v_Data.WorldPosition, 1.0f);
}

#shader fragment
#version 330 core

/* Data Recieved from the vertex shader */
in struct Data
{
	vec3 Normal;
	vec2 TexCoord;
	vec3 WorldPosition;
	vec4 LightSpacePos;
} v_Data;

uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

uniform int u_TerrainWidth;	/* In the x - direction */
uniform int u_TerrainDepth;	/* In the z - direction */

/* Structure for the ambient light source */
struct DirectionalLight
{
	vec4 Color;
	vec3 Direction;
	float Intensity;
};

/* Ambient Light (Represents Sun) */
uniform DirectionalLight u_LightSource = DirectionalLight(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);

uniform bool u_CalculateShadow = false;

uniform float u_AmbientStrength;
uniform float u_Shininess;
uniform float u_Reflectivity;

/* RGB map representing the amount of textures to be used to fill the terrain */
uniform sampler2D u_BlendMap;

/* Base Texture for the terrain */
uniform sampler2D u_Texture0;

/* Represented by the R - channel in blendmap */
uniform sampler2D u_Texture1;

/* Represented by the G - channel in blendmap */
uniform sampler2D u_Texture2;

/* Represented by the B - channel in blendmap */
uniform sampler2D u_Texture3;

uniform sampler2D u_ShadowMap;

/* Final color */
out vec4 fColor;

float ShadowCalculation(vec4 fragLightSpacePos)
{
	vec3 ProjectionCoords = fragLightSpacePos.xyz / fragLightSpacePos.w;	/* Normalised coordinates [-1, 1] */
	ProjectionCoords = (ProjectionCoords + 1.0f) / 2.0f;					/* Range 0 - 1 */
	float ClosestDepth = texture(u_ShadowMap, ProjectionCoords.xy).r;		/* ProjectionCoords.z is the current depth of the fragment */

	// Check whether the fragment is in the shade or light
	return ClosestDepth > ProjectionCoords.z ? 0.0f : 1.0f;
}

void main()
{
	// Normalize the light vector and the normal vector
	vec3 UnitNormal = normalize(v_Data.Normal);

	// Calculate Ambient Color
	vec4 AmbientColor = u_AmbientStrength * u_LightSource.Color;

	/**** Color due to the Global Light Source ****/
	// Calculate Diffuse Color
	vec3 LightDir = normalize(-u_LightSource.Direction);
	float brightness = max(0.0f, dot(UnitNormal, LightDir));
	vec4 DiffuseColor_Global = u_LightSource.Color * brightness;

	// Calculate the specular color
	vec3 ViewDir = normalize(u_CameraPos - v_Data.WorldPosition);
	vec3 ReflectedDir = reflect(-LightDir, UnitNormal);
	float Shine = pow(max(dot(ReflectedDir, ViewDir), 0.0f), u_Shininess);
	vec4 SpecularColor_Global = u_LightSource.Color * Shine * u_Reflectivity;


	/**** Color due to the Point Lights ****/
	// Diffuse Light Color
	vec3 UnitLightVec = normalize(u_LightPos - v_Data.WorldPosition);
	brightness = max(0.0f, dot(UnitNormal, UnitLightVec));
	vec4 diffuseColor = u_LightColor * brightness;

	// Specular Light Color
	ViewDir = normalize(u_CameraPos - v_Data.WorldPosition);
	vec3 ReflectedLightDir = reflect(-UnitLightVec, UnitNormal);
	float shine = pow(max(dot(ReflectedLightDir, ViewDir), 0.0), u_Shininess);
	vec4 specularColor = shine * u_Reflectivity * u_LightColor;

	// Calculate the shadow
	float Shadow = 0.0f;
	if(u_CalculateShadow)
		Shadow = ShadowCalculation(v_Data.LightSpacePos);

	vec2 BlendTexCoord = vec2(v_Data.TexCoord.x / (u_TerrainWidth - 1), v_Data.TexCoord.y / (u_TerrainDepth - 1));
	vec4 texColorBlend = texture(u_BlendMap, BlendTexCoord);

	vec4 texColor0 = texture(u_Texture0, v_Data.TexCoord) * (1.0f - (texColorBlend.r + texColorBlend.g + texColorBlend.b));
	vec4 texColor1 = texture(u_Texture1, v_Data.TexCoord) * texColorBlend.r;
	vec4 texColor2 = texture(u_Texture2, v_Data.TexCoord) * texColorBlend.g;
	vec4 texColor3 = texture(u_Texture3, v_Data.TexCoord) * texColorBlend.b;

	fColor = (AmbientColor + (1.0f - Shadow) * (DiffuseColor_Global + SpecularColor_Global + diffuseColor + specularColor)) * (texColor0 + texColor1 + texColor2 + texColor3);
}
