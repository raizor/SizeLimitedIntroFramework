// Trendwhore.frag

varying vec3 norm;
varying vec3 worldPos;

uniform vec3 Color;

uniform vec3 LightDir;

void main()
{
	//gl_FragData[0] = vec4(1.0);
	gl_FragData[0] = vec4(worldPos, 1.0f); // world pos
	gl_FragData[1] = vec4(Color * vec3(dot(normalize(norm), normalize(LightDir)) * .5 + .5), 1); // color - color of cube
	gl_FragData[2] = vec4(norm, 1.0); // normal - rotation of cube
}
