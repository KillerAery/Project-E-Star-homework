$input a_position, a_normal, a_texcoord0
$output v_wpos, v_wnormal, v_texcoord0

#include "common.sh"

void main()
{
	// v_wpos
	v_wpos = mul(u_model[0], vec4(a_position, 1.0) ).xyz;
	// v_wnormal
	vec3 normal = a_normal * 2.0 - 1.0;
	vec3 wnormal = mul(u_model[0], vec4(normal.xyz, 0.0) ).xyz;
	v_wnormal = normalize(wnormal);
	// v_texcoord
	v_texcoord0 = a_texcoord0;
	// gl_Position
	gl_Position = mul(u_viewProj, vec4(v_wpos, 1.0));
}
