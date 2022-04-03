$input v_wpos, v_wnormal, v_texcoord0

#include "common.sh"

SAMPLER2D(s_texColor,  0);
uniform vec4 u_k;
uniform vec4 u_lightPos;
uniform vec4 u_ambientIntensity;
uniform vec4 u_lightIntensity;

vec3 blinnphong(vec3 _wpos, vec3 _wnormal, vec3 _viewDir)
{
	// 解压 u_k => ambient系数、diffuse系数、specular系数、specular幂次
	float ka = u_k.x;
	float kd = u_k.y;
	float ks = u_k.z;
	float p = u_k.w;
	// ambient
	vec3 ambient = ka* u_ambientIntensity.xyz;
	// diffuse
	vec3 lightDir = u_lightPos.xyz - _wpos;
	float distSq = dot(lightDir,lightDir);
	lightDir = normalize(lightDir);
	vec3 diffuse = kd * u_lightIntensity.xyz / distSq * max(dot(_wnormal,lightDir),0.0);
	// specular
	vec3 h = normalize(_viewDir+lightDir);
	vec3 specular = ks * u_lightIntensity.xyz / distSq * pow(max(dot(_wnormal,h),0.0),p);
	return ambient + diffuse + specular;
}

void main()
{
	vec3 wnormal = normalize(v_wnormal);
	vec3 eyePos = mul(vec4(0.0, 0.0, 0.0, 1.0), u_view).xyz;
	vec3 eyedir = normalize(eyePos - v_wpos);

	vec4 color = toLinear(texture2D(s_texColor, v_texcoord0));

	gl_FragColor.xyz = blinnphong(v_wpos,wnormal,eyedir) * color.xyz;
	gl_FragColor.w = 1.0;
}
