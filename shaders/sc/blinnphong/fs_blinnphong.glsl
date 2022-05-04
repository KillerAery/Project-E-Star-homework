$input v_wpos, v_wnormal, v_texcoord0

#include "common.sh"

SAMPLER2D(s_texColor,  0);
uniform vec4 u_k;
uniform vec4 u_lightDir;
uniform vec4 u_ambient;
uniform vec4 u_lightRadiance;

vec3 blinnphong(vec3 _wpos, vec3 _wnormal, vec3 _viewDir)
{
	// 解压 u_k => diffuse系数、specular系数、ambient系数、specular幂次
	float kd = u_k.x;
	float ks = u_k.y;
	float ka = u_k.z;
	float p = u_k.w;
	// ambient
	vec3 ambient = ka* u_ambient.xyz;
	// diffuse
	vec3 lightDir = u_lightDir;
	vec3 diffuse = kd * u_lightRadiance.xyz * max(dot(_wnormal,lightDir),0.0);
	// specular
	vec3 h = normalize(_viewDir+lightDir);
	vec3 specular = ks * u_lightRadiance.xyz * pow(max(dot(_wnormal,h),0.0),p);
	return ambient + diffuse + specular;
}

void main()
{
	vec3 wnormal = normalize(v_wnormal);
	vec3 eyePos = mul(vec4(0.0, 0.0, 0.0, 1.0), u_view).xyz;
	vec3 eyedir = normalize(eyePos - v_wpos);

	vec3 color = 
	blinnphong(v_wpos,wnormal,eyedir) *
	toLinear(texture2D(s_texColor, v_texcoord0)).xyz;

  	color = color / (color + vec3(1.0,1.0,1.0));
  	color = pow(color, vec3(1.0/2.2,1.0/2.2,1.0/2.2)); 
  	gl_FragColor = vec4(color, 1.0);
}
