uniform sampler2D t;
uniform sampler2D iChannel0;          // input channel. XX = 2D/Cube
uniform sampler3D iChannel3d;          // input channel. XX = 2D/Cube
uniform sampler2D iChannel1;          // input channel. XX = 2D/Cube
uniform sampler2D iChannel2;          // input channel. XX = 2D/Cube

void main()
{
	vec2 p = gl_FragCoord.xy / vec2(800.0, 600.0);
	gl_FragData[0] = vec4(p.x, p.y, 0.0, 1.0);
	gl_FragData[1] = vec4(p.y, 1.0, 0.0, 1.0);
	gl_FragData[2] = vec4(p.x, 1.0, 0.0, 1.0);

	gl_FragDepth = p.y;
}