$input v_wpos, v_tangent, v_bitangent, v_normal, v_texcoord0

#include "common.sh"

SAMPLER2D(s_texColor,  	0);
SAMPLER2D(s_texNormal,	1);
SAMPLER2D(s_texAorm,  	2);
SAMPLERCUBE(s_texCube, 	3);
SAMPLERCUBE(s_texCubeIrr, 4);
SAMPLER2D(s_texLUT,  	5);

uniform vec4 u_eyePos;
uniform vec4 u_lightDir;
uniform vec4 u_lightRadiance;
uniform vec4 u_k;
uniform mat4 u_mtx;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float NdotH = max(dot(N, H), 0.0);
	float denom = (NdotH * NdotH * (a*a - 1.0) + 1.0);
	return max(a*a/(PI*denom*denom),0.001);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float a = (roughness + 1.0);
	float k = (a * a) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NoV = max(dot(N,V),0.0);
    float NoL = max(dot(N,L),0.0);
    float ggx2 = GeometrySchlickGGX(NoV, roughness);
    float ggx1 = GeometrySchlickGGX(NoL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(vec3 F0, vec3 V, vec3 H)
{
    float t = 1.0-max(dot(V,H),0.0);
	return F0 + (1.0 - F0) * pow(t,5.0);
}

void main()
{
	// albedo
  	vec3 albedo = toLinear(texture2D(s_texColor, v_texcoord0).rgb);
	// 切线空间下的 texture normal
	vec3 normal;
	normal.xy = texture2D(s_texNormal, v_texcoord0).xy * 2.0 - 1.0;
	normal.z = sqrt(1.0-dot(normal.xy,normal.xy));
	// 世界空间下的 texture normal
	//mat3 tbn = mtxFromCols(normalize(v_tangent), normalize(v_bitangent), normalize(v_normal));
	mat3 tbn = mtxFromCols(normalize(v_tangent), normalize(v_bitangent), normalize(v_normal));
	normal = mul(tbn,normal);
	normal = normalize(normal);
  	// ambient occlusion & roughness & metallic
  	vec3 aorm = texture2D(s_texAorm, v_texcoord0).rgb;
  	float ao = aorm.x;
  	float roughness = aorm.y;
  	float metallic = aorm.z;
	// view
	vec3 V = normalize(u_eyePos.xyz - v_wpos);
	vec3 N = normalize(normal);
  	float NdotV = max(dot(N, V), 0.0);
  	vec3 L = normalize(u_lightDir.xyz);
  	vec3 H = normalize(V + L);
  	float NdotL = max(dot(N, L), 0.0); 

	// ---------------------- Direct Lighting --------------------
	// Normal Distribution Function
  	float NDF = DistributionGGX(N, H, roughness); 
	// Geometry
  	float G   = GeometrySmith(N, V, L, roughness);
	// Fresnel
	vec3 F0 = vec3(0.04,0.04,0.04); 
  	F0 = mix(F0, albedo, metallic);
  	vec3 F = fresnelSchlick(F0, V, H);
	// Cook-Torrance specular BRDF
  	vec3 numerator    =  NDF * G * F; 
  	float denominator = max((4.0 * NdotL * NdotV), 0.001);
  	vec3 specular =  numerator / denominator;
	// diffuse BRDF
	vec3 diffuse = max(albedo / PI, 0.0001);
	// direct
	float kd = u_k.x * (1.0-metallic);
	float ks = u_k.y;
  	vec3 radiance = u_lightRadiance.xyz;
  	vec3 direct = (ks * specular + kd * diffuse) * radiance * NdotL;
	
	// ----------------- Indirect Lighting（Split Sum） -------------
	float mip = 1.0 + 5.0*(1.0 - metallic); // Use mip levels [1..6] for radiance.
	vec3 VR = 2.0 * NdotV * N - V; // Same as: -reflect(V, N);
	vec3 cubeR = normalize(mul(u_mtx, vec4(VR,0.0)).xyz);
	cubeR = fixCubeLookup(cubeR, mip, 256.0);
	vec3 envIrradiance    = toLinear(textureCubeLod(s_texCube, cubeR, mip).xyz);

	vec3 envDiffuse  = albedo * envIrradiance * kd;
	
	float brdfIntergrate = texture2D(s_texLUT, vec2(roughness,NdotV)).x;	//lut
	vec3 envSpecular = (1.0-brdfIntergrate) * envIrradiance * ks;

	vec3 indirect    = (envDiffuse + envSpecular) * ao;

	// FragColor
  	vec3 color = direct + indirect;
  	color = color / (color + vec3(1.0,1.0,1.0));
  	color = pow(color, vec3(1.0/2.2,1.0/2.2,1.0/2.2)); 
  	gl_FragColor = vec4(color, 1.0);
}
