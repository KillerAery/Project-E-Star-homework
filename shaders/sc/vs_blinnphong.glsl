$input a_position, a_normal, a_texcoord0
$output v_wpos, v_normal, v_texcoord0

#include "common.sh"

uniform mat4 u_lightMtx;

void main()
{
	// v_wpos
	v_wpos = mul(u_model[0], vec4(a_position, 1.0) );
	// v_normal
	vec3 normal = a_normal.xyz * 2.0 - 1.0;
	vec3 wnormal = mul(u_model[0], vec4(normal.xyz, 0.0) ).xyz;
	v_normal = normalize(wnormal);
	// v_texcoord
	vec2 texcoord = a_texcoord0;
	texcoord.y = 1.0-texcoord.y;
	v_texcoord0 = texcoord;
	// gl_Position
	gl_Position = mul(u_viewProj, v_wpos);
}
