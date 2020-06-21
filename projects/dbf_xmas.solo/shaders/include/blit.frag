uniform sampler2D t;

void main()
{
	vec2 p = gl_FragCoord.xy / vec2(800.0, 600.0);
	gl_FragColor = vec4(texture2D(t, p).xyz, 1.0);
}