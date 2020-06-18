uniform vec3 color;
uniform vec3 light;
uniform float depthPass;

varying vec3 vColor;
varying vec3 vNormal;
varying vec3 light0Color;
varying float extra;

vec4 pack_depth( const in float depth ) {

	const vec4 bit_shift = vec4( 256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0 );
	const vec4 bit_mask  = vec4( 0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0 );
	vec4 res = fract( depth * bit_shift );
	res -= res.xxyz * bit_mask;
	return res;

}

void main() {

	float depth = gl_FragCoord.z / gl_FragCoord.w;
	float near = 400.0;
	float far = 600.0;
	float depthcolor = 1.0 - smoothstep( near, far, depth );

	vec3 l = light;
	l = normalize(l);
	float d0 = max(0.25,dot(vNormal, l));

	vec3 extraColor = vec3(1.0, 1.0-extra, 1.0);

	if (depthPass == 1.0) {
		gl_FragColor = pack_depth( gl_FragCoord.z );
	} else {
		gl_FragColor = vec4( color * extraColor * vColor.xyz * d0 * depthcolor, 1.0 );
	}
	
}