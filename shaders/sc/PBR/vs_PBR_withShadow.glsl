$input a_position, a_tangent, a_normal, a_texcoord0
$output v_wpos, v_tangent, v_bitangent, v_normal, v_texcoord0, v_lpos

#include "common.sh"

uniform mat4 u_lightMtx;

void main()
{
	// v_wpos
	v_wpos = mul(u_model[0], vec4(a_position.xyz, 1.0) );

	// v_tangent v_normal v_bitangent
	vec3 normal = a_normal.xyz;
	vec3 tangent = cross(normal,normalize(vec3(0.0,1.0,1.0)));
	if(dot(tangent,tangent)<0.1){
		tangent = cross(normal,normalize(vec3(1.0,1.0,0.0)));
	}
	vec3 wnormal = mul(u_model[0], vec4(normal, 0.0) ).xyz;
	vec3 wtangent = mul(u_model[0], vec4(tangent, 0.0) ).xyz;
	v_normal = normalize(wnormal);
	v_tangent = normalize(wtangent);
	v_bitangent = cross(v_normal, v_tangent);

	// albedo texture
	vec2 texcoord = a_texcoord0;
	texcoord.y = 1.0-texcoord.y;
	v_texcoord0 = texcoord;

	// position from light,used for shadow
	v_lpos = mul(u_lightMtx, v_wpos);

	// gl_Position
	gl_Position = mul(u_viewProj, v_wpos);
}
