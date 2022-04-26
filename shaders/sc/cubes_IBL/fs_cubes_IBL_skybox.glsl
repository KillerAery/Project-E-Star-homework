$input v_dir

#include "common.sh"

SAMPLERCUBE(s_texCube, 	0);

void main()
{
	vec3 dir = normalize(v_dir);

	// LOD 0 = radiance map
	float lod = 0.0;
	dir = fixCubeLookup(dir, lod, 256.0);
	vec4 color = toLinear(textureCubeLod(s_texCube, dir, lod));
	gl_FragColor = toFilmic(color);
}
