// Trendwhore.frag

varying vec3 nurmull;
varying vec3 worldPos;

uniform vec3 Color;

uniform vec3 LightDir;

void main()
{
	gl_FragData[0] = vec4(worldPos, 1.0f);
	gl_FragData[1] = vec4(Color * vec3(dot(normalize(nurmull), normalize(LightDir)) * .5 + .5), 1);
}
