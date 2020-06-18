// Noise3DLinear.frag
uniform sampler2D t;
uniform int textureLayer;

/*
float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);
	
	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
	vec2 rg = texture2D( iChannel0, (uv+ 0.5)/256.0, -100.0 ).yx;
	return mix( rg.x, rg.y, f.z );
}z

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
	
    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}
*/

float hash( float n ) { return fract(sin(n)*43758.5453123); }

float noise( in vec3 x )
{
	vec2 rg = texture2D( t, x.xy, -100.0 ).yx;
	return mix( rg.x, rg.y, x.z );		
/*
    vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);
	
	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
	vec2 rg = texture2D( t, (uv+ 0.5)/256.0, -100.0 ).yx;
	return mix( rg.x, rg.y, f.z );
	*/

}

void main()
{

	/*
    gl_FragData[0] = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);*/

	vec2 iResolution = vec2(800.0, 600.0);

	//vec2 q = gl_FragCoord.xy / iResolution.xy;
	//vec2 p = -1.0 + 2.0*q;
	//p.x *= iResolution.x / iResolution.y;
	vec2 p = gl_FragCoord.xy / iResolution;
	//gl_FragColor = vec4(1.0);

	gl_FragData[0].r = noise(vec3(p, textureLayer / 255.0));

	//gl_FragData[0].rgb = sin(n)*43758.5453123;
	//gl_FragData[0].a = 1.0;
}


/*
float s(vec3 x){
	return texture3D(t,x/256.).x;
}
void main(){	
float x=float(a);
	gl_FragData[0]=vec4(1)*(
		s(vec3(gl_FragCoord.xy,x))*.5+
		s(vec3(gl_FragCoord.xy,x)*2.)*.25+
		s(vec3(gl_FragCoord.xy,x)*4.)*.125+
		s(vec3(gl_FragCoord.xy,x)*8.)*.0625+
		s(vec3(gl_FragCoord.xy,x)*16.)*.03125

	);
}
*/