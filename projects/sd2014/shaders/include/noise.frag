uniform sampler3D t;
uniform float time;

/*
float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);
	f = f*f*(3.0-2.0*f);
	
	vec2 uv = (p.xy) + f.xy;
	return texture2D( t, (uv+ 0.5)/256.0, -100.0 ).x;
}

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);
	
	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
	vec2 rg = texture2D( t, (uv+ 0.5)/256.0, -100.0 ).yx;
	return mix( rg.x, rg.y, f.z );
}

float hash( in float n )
{
    return fract(sin(n)*43758.5453);
}
*/
void main()
{
	//vec2 p = gl_FragCoord.xy / vec2(800.0, 600.0);
	//gl_FragColor = vec4(texture2D(t, p).xyz, 1.0);	
	vec2 p = gl_FragCoord.xy / vec2(800.0, 600.0);
	vec3 pos = vec3(p.xy, time*0.01);
	float nz = texture3D(t, pos);
	float n1 = 0;//noise((p.xy + time) * 4);
	vec4 col = vec4(nz, nz, nz, 1.0);
	gl_FragColor = col;
}