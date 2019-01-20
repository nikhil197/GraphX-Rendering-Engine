#shader vertex
#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec4 vColor;

/* Uniforms */
uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Projection = mat4(1.0f);
uniform mat3 u_Normal = mat3(1.0f);

/* Data sent to the fragment shader */
out struct Data
{
	vec3 Normal;
	vec3 WorldPosition;
	vec4 Color;
} v_Data;

void main()
{
	v_Data.WorldPosition = vec3(u_Model * vec4(vPosition, 1.0f));
	gl_Position = u_Projection * u_View * v_Data.WorldPosition;
	v_Data.Normal = u_Normal * vNormal;
	v_Data.Color = vColor;
}

#shader fragment
#version 330 core

/* Metallic properties of the object */
struct Material
{
	float Shininess;
	float Reflectivity;
	vec4 Color;	// Base Color of the object
};

/* Data Recieved from the vertex shader */
in struct Data
{
	vec3 Normal;
	vec3 WorldPosition;
	vec4 Color;
} v_Data;

/* Structure for the ambient light source */
struct DirectionalLight
{
	vec4 Color;
	vec3 Direction;
	float Intensity;
};

/* Structure for the point lights */
struct PointLight
{
	vec3 Position;
	vec4 Color;
	vec3 AttenuationFactors;
	float Intensity;
};

/* Uniforms */
uniform float u_AmbientStrength = 0.01f;

uniform vec3 u_CameraPos;

/* Ambient Light (Represents Sun) */
uniform DirectionalLight u_LightSource = DirectionalLight(vec3(0.0f, 100.f, -50.f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 1.0f);

/* Point Light */
uniform PointLight u_PointLight = PointLight(vec3(0.0f, 0.0f, 10.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), 1.0f);

/* Material for the object */
uniform Material u_Material = Material(256.0f, 1.0f, vec4(1.0f, 1.0f, 1.0f, 1.0f));

/* Final color */
out vec4 fColor;

void main()
{
	vec3 UnitNormal = normalize(v_Data.Normal);

	// Calculate Ambient Color
	vec4 AmbientColor = u_AmbientStrength * u_LightSource.Color;

	// Calculate the attenuation factors
	float distance = length(v_Data.WorldPosition - u_PointLight.Position);
	float AttenuationFactor = (u_PointLight.AttenuationFactors.x + u_PointLight.AttenuationFactors.y * distance + u_PointLight.AttenuationFactors.z * distance * distance);

	/**** Color due to the Global Light Source ****/
	// Calculate Diffuse Color
	vec3 LightDir = normalize(-u_LightSource.Direction);
	float brightness = max(0.0f, dot(UnitNormal, LightDir));
	vec4 DiffuseColor_Global = u_LightSource.Color * brightness;

	// Calculate the specular color
	vec3 ViewDir = normalize(u_CameraPos - v_Data.WorldPosition);
	vec3 ReflectedDir = reflect(-LightDir, UnitNormal);
	float Shine = pow(max(dot(ReflectedDir, ViewDir), 0.0f), u_Material.Shininess);
	vec4 SpecularColor_Global = u_LightSource.Color * Shine * u_Material.Reflectivity;


	/**** Color due to the Point Lights ****/
	// Calculate Diffuse Color
	LightDir = normalize(u_PointLight.Position - v_Data.WorldPosition);
	brightness = max(0.0f, dot(UnitNormal, LightDir));
	vec4 DiffuseColor_Point = (u_PointLight.Color * brightness) / AttenuationFactor ;

	// Calculate Specular Color
	ReflectedDir = reflect(-LightDir, UnitNormal);
	Shine = pow(max(dot(ReflectedDir, ViewDir), 0.0f), u_Material.Shininess);
	vec4 SpecularColor_Point = (u_PointLight.Color * Shine * u_Material.Reflectivity) / AttenuationFactor;

	// Calculate the final color
	fColor = (AmbientColor + (SpecularColor_Global + SpecularColor_Point) + (DiffuseColor_Global + DiffuseColor_Point)) * v_Data.Color;
}