#include "common.sh"

float hardShadow(sampler2DShadow s_shadowmap,vec4 v_lpos){
	vec3 texCoord = v_lpos.xyz/v_lpos.w;
  	float bias = 0.005;
	float visibility = shadow2D(s_shadowmap, vec3(texCoord.xy, texCoord.z-bias));
    return visibility;
}