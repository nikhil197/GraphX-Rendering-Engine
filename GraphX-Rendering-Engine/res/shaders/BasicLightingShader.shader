#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColor;

// Uniform matrices for the transformations
uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_Projection = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat3 u_Normal = mat3(1.0f);

uniform mat4 u_LightSpaceMatrix = mat4(1.0f);

// Varying variables
out struct Data
{
	vec3 Normal;
	vec3 WorldPosition;
	vec4 Color;
	vec4 LightSpacePos;
} v_Data;

void main()
{
	v_Data.WorldPosition = vec3(u_Model * vec4(vPosition, 1.0));
	gl_Position = u_Projection * u_View * vec4(v_Data.WorldPosition, 1.0);
	v_Data.Normal = u_Normal * vNormal;
	v_Data.Color = vColor;
	v_Data.LightSpacePos = u_LightSpaceMatrix * vec4(v_Data.WorldPosition, 1.0f);
}

#shader fragment
#version 330 core

in struct Data
{
	vec3 Normal;
	vec3 WorldPosition;
	vec4 Color;
	vec4 LightSpacePos;
} v_Data;

// Uniforms
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;
uniform vec4 u_TintColor = vec4(0.0f);

/* Structure for the ambient light source */
struct DirectionalLight
{
	vec4 Color;
	vec3 Direction;
	float Intensity;
};

/* Ambient Light (Represents Sun) */
uniform DirectionalLight u_LightSource = DirectionalLight(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);

uniform float u_AmbientStrength;
uniform float u_Shininess;
uniform float u_Reflectivity;

uniform vec3 u_AttenuationFactors = vec3(1.0, 0.0, 0.0);

uniform sampler2D u_ShadowMap;
uniform sampler2D u_Texture0;

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
	bool bCalculateShadow = false;

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

	// Calculate the distance of the fragment from the light source
	float distance = length(u_LightPos - v_Data.WorldPosition);

	// Calculate the attenuation factor based on the distance of the fragment from the light source
	float AttenuationFactor = (u_AttenuationFactors.x + (u_AttenuationFactors.y * distance) + (u_AttenuationFactors.z * distance * distance));

	// Calculate the shadow
	float Shadow = 0.0f;
	if(bCalculateShadow)
		Shadow = ShadowCalculation(v_Data.LightSpacePos);

	// Divide the diffuse and specular components of the light color (ambient is the property of the environment, probably due to the directional light source - most probably sun)
	fColor = (AmbientColor + (1.0f - Shadow) * (DiffuseColor_Global + SpecularColor_Global + (diffuseColor / AttenuationFactor) + (specularColor / AttenuationFactor))) * (mix(texture(u_Texture0, vec2(v_Data.Color.xy)), u_TintColor, 0.5f));
}