/*
	AR: 2.76 : 1
	AR: 1.85 : 1
*/

//----------------------------------------------------------------

varying vec2 uv;
uniform float time;

uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

uniform sampler2D mask;

varying mat4 mview;

varying float znear;
varying float zfar;

varying float nearPlane;
varying float farPlane;

varying mat4 pmat;

uniform vec3 sky1;
uniform vec3 sky2;

uniform vec3 lig;

uniform vec3 cube;
uniform vec3 objectPos;
uniform float objectMix;
varying vec3 cameraPosition;
varying float nearPlanePosition;
varying vec3 viewDirection;
varying mat4 mviewproj;
#define pi2 6.28318530718

const float precis = 0.015;

// Camera uniforms
uniform vec2 g_resolution;// = vec2(800.0, 600.0);

struct hitInfo
{
	
	int materialId;
};

float hash( float n )
{
    return fract(sin(n)*43758.5453123);
}


float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);

	float a = texture2D( iChannel0, x.xy/256.0 + (p.z+0.0)*120.7123 ).x;
    float b = texture2D( iChannel0, x.xy/256.0 + (p.z+1.0)*120.7123 ).x;
	return mix( a, b, f.z );

	/*
    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0 + 113.0*p.z;

    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
				   */
}

const mat3 m = mat3( 0.00,  0.80,  0.60,
                    -0.80,  0.36, -0.48,
                    -0.60, -0.48,  0.64 );

float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);

	vec2 uv = p.xy + f.xy*f.xy*(3.0-2.0*f.xy);

	return -1.0 + 2.0*texture2D( iChannel0, (uv+0.5)/256.0, -100.0 ).x;
}

vec3 texturize( sampler2D sa, vec3 p, vec3 n )
{
	vec3 x = texture2D( sa, p.yz ).xyz;
	vec3 y = texture2D( sa, p.zx ).xyz;
	vec3 z = texture2D( sa, p.xy ).xyz;

	return x*abs(n.x) + y*abs(n.y) + z*abs(n.z);
}

float fbm( vec3 p )
{
    float f;
    f  = 0.5000*noise( p ); p = m*p*2.02;
    f += 0.2500*noise( p ); p = m*p*2.03;
    f += 0.1250*noise( p ); p = m*p*2.01;
    f += 0.0625*noise( p );
    return f;
}

//----------------------------------------------------------------

mat3 rotz(in float a)
{
	float ca = cos(a);
	float sa = sin(a);
	return mat3(
		ca,sa,0.0,
		-sa,ca,0.0,
		0.0,0.0,1.0
	);
}

mat3 rotx(in float a)
{
	float ca = cos(a);
	float sa = sin(a);
	return mat3(
		1.0,0.0,0.0,
		0.0,ca,-sa,
		0.0,sa,ca
	);
}

mat3 roty(in float a)
{
	float ca = cos(a);
	float sa = sin(a);
	return mat3(
		ca,0.0,sa,
		0.0,1.0,0.0,
		-sa,0.0,ca
	);
}

//----------------------------------------------------------------

vec2 opUnion( vec2 d1, vec2 d2 )
{
	return (d1.x<d2.x) ? d1 : d2;
}
float opSub( float d1, float d2 )
{
    return max(-d2,d1);
}

vec3 opRep( vec3 p, vec3 c )
{
    return mod(p,c)-0.5*c;
}

float length2( vec2 p )
{
	return sqrt( p.x*p.x + p.y*p.y );
}

float length6( vec2 p )
{
	p = p*p*p; p = p*p;
	return pow( p.x + p.y, 1.0/6.0 );
}

float length8( vec2 p )
{
	p = p*p; p = p*p; p = p*p;
	return pow( p.x + p.y, 1.0/8.0 );
}

float sdTorus82( vec3 p, vec2 t )
{
  vec2 q = vec2(length2(p.xz)-t.x,p.y);
  return length8(q)-t.y;
}

float sdCylinder( vec3 p, vec2 h )
{
  return max( length(p.xz)-h.x, abs(p.y)-h.y );
}

float udRoundBox( vec3 pos, vec3 size, float r )
{
  return length(max(abs(pos)-size,0.0))-r;
}

float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}

float sdCone( vec3 p, vec2 c )
{
    float q = length(p.xz);
    return max( dot(c,vec2(q,p.y)), p.y );
}

float sdSphere( in vec3 p, in vec4 e )
{
	vec3 di = p - e.xyz;
	return length(di) - e.w;
}

vec3 opTwist( vec3 p )
{
    float  c = cos(10.0*p.y+10.0);
    float  s = sin(10.0*p.y+10.0);
    mat2   m = mat2(c,-s,s,c);
    return vec3(m*p.xz,p.y);
}

vec2 sdSegment( vec3 a, vec3 b, vec3 p )
{
	vec3 pa = p - a;
	vec3 ba = b - a;
	float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
	
	return vec2( length( pa - ba*h ), h );
}

//----------------------------------------------------------------

float smin( float a, float b )
{
    float k = 0.06;
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( b, a, h ) - k*h*(1.0-h);
}

float smin( float a, float b, float k )
{
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( b, a, h ) - k*h*(1.0-h);
}

vec2 smin( vec2 a, vec2 b )
{
    float k = 1.6;
	float h = clamp( 0.5 + 0.5*(b.x-a.x)/k, 0.0, 1.0 );
	return mix( b, a, h ) - k*h*(1.0-h);
}


float sdCylinder6( vec3 p, vec2 h )
{
  return max( length6(p.xz)-h.x, abs(p.y)-h.y );
}

//sdCapsule(   pos,vec3(-1.3,0.20,-0.1), vec3(-1.0,0.20,0.2), 0.1  ), 31.9 )
float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
	vec3 pa = p - a;
	vec3 ba = b - a;
	float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
	
	return length( pa - ba*h ) - r;
}

//----------------------------------------------------------------

#define MAT_NONE 0.0
#define MAT_DBF 1.0
#define MAT_GROUND 2.0
#define MAT_TREE 3.0
#define MAT_SNOW 4.0

#define MAT_HAT 5.0
#define MAT_EYE 6.0
#define MAT_BODY 7.0
#define MAT_NOSE 8.0
#define MAT_ARM 9.0

#define MAT_D 10.0
#define MAT_B 11.0
#define MAT_F 12.0

//----------------------------------------------------------------

float terrain( vec2 x )
{
	vec2 z = x*0.05;
	
	x *= 0.06*1.0; 
	x += 227.3;
	
	vec2 p = floor(x);
    vec2 f = fract(x);

    f = f*f*(3.0-2.0*f);
    float a = texture2D(iChannel0,(p+vec2(0.5,0.5))/256.0,-32.0).x;
	float b = texture2D(iChannel0,(p+vec2(1.5,0.5))/256.0,-32.0).x;
	float c = texture2D(iChannel0,(p+vec2(0.5,1.5))/256.0,-32.0).x;
	float d = texture2D(iChannel0,(p+vec2(1.5,1.5))/256.0,-32.0).x;
	float height =mix(mix( a, b,f.x), mix( c, d,f.x), f.y);

    height -= 0.04*(noise( 5.0*z ));

	height -= sin(z).x;
	height += sin(z).y;

	height -= sin(z*0.3).x;
	height += sin(z*0.5).y;
		
	//r = r*35.0;
	height = height*5.0;

	// main channel cutout (X)
	float zp = opRep(vec3(z, 0.0), vec3(1.0, 1.0, 1.0));
	float ss = smoothstep( 0.5, 2.2, abs(z.y) );
	height = mix( height, height-5.0, ss );
	
	// channel cutout (Z)
	float cc = 1.0-smoothstep( 0.1, 1.0, abs(z.x) );
	cc *= smoothstep( 0.5, 1.0, abs(sin(z.y)) );
	height = mix( height, height-5.0, cc );
	
	/*
	// channel cutout
	float ss = smoothstep( 0.5, 2.2, abs(z.y) );
	height = mix( height, -3.0, 1.0-ss );
	
	// channel cutout 2
	float cc = 1.0-smoothstep( 0.1, 1.0, abs(z.x) );
	cc *= smoothstep( 0.5, 1.0, abs(z.y) );
	height = mix( height, height-4.0, cc );
	*/
	return height-0.2;
}

vec2 distToDbf(in vec3 pos) // returns dist, materal-id
{
	float materialId = MAT_SNOW;
	mat3 rot = rotx(pi2 * 0.25);
	pos.y-=40.0;
		
	// d
	float d = sdTorus82(rot*pos, vec2(10.0, 3.2));
	d = min(d, udRoundBox(pos+vec3(-10.0, -12.0, 0.0), vec3(2.20, 12.0, 2.2), 1.0));
		
	// b
	pos.x-=28.0;
	float b = sdTorus82(pos*rot, vec2(10.0, 3.2));
	b = min(b, udRoundBox(pos+vec3(10.0, -12.0, 0.0), vec3(2.20, 12.0, 2.2), 1.0));

	// f
	pos.x -= 28.0;
	float f = udRoundBox(pos+vec3(10.0, 1.0, 0.0), vec3(2.20, 10.0, 2.2), 1.0);
	pos+=vec3(0.0, -9.0, 0.0);
	f = min(f, sdTorus82(rot*pos, vec2(10.0, 3.2)));

	// cut f
	f = opSub(f, sdBox(pos + vec3(-4.8, 10.0, 1.0), vec3(12.0, 10.0, 5.0)));
	f = opSub(f, sdBox(pos + vec3(-14.8, -8.0, 1.0), vec3(12.0, 10.0, 5.0)));
	f = min(f, udRoundBox(pos+vec3(5.0, 7.5, 0.0), vec3(2.2, 2.20, 2.2), 1.0));
	float dist = min(min(f, d),b);
	/*
	if (dist == d)
	{
		materialId = MAT_D;
	}else if (dist == b)
	{
		materialId = MAT_B;
	}else if (dist == f)
	{
		materialId = MAT_F;
	}*/

	
	vec2 ret = vec2(dist, materialId);
	return ret;
}

float trees( vec3 p, float h )
{
	float e = smoothstep( 0.4, 0.6, texture2D(iChannel1,0.001*p.xz, -32.0).x );

	
	e *= smoothstep( 23.0, 24.0, abs(p.z-3.0) );
	
    vec2 c = floor( p.xz/4.0 );
	
	p.xz = mod( p.xz, 4.0 ) - 2.0;
	p.xz += 3.0*(-1.0+2.0*texture2D( iChannel2, c/256.0, -32.0 ).xy);
		

	float r = sdCone( p-vec3(0.0,h+6.0*e,0.0), vec2(0.9397,0.342) );
	return r;
} 

float crags( vec3 p, float h )
{
	float e = smoothstep( 0.1, 0.9, texture2D(iChannel0, 0.0002*p.xz, -32.0).x );                
	h+=e*2.0;
	return p.y - h;
	
	//e *= smoothstep( 1.0, 2.0, abs(p.z-3.0) );       
		
	//p.xz = mod( p.xz, 4.0 ) - 2.0;
	p.xz = mod( p.xz * vec2(0.1, 0.1), 4.0 ) - 2.0;
	float crags = sdCone( p-vec3(0.0, h+3.0*e, 0.0), vec2(0.9397,0.642) );
                				
	crags *= 0.4;
    
	return crags;
}

//vec3 lig = normalize(vec3(-0.5,0.25,-0.3));


vec2 distToSnowman(in vec3 pos, in vec3 offset) // returns dist, materal-id
{
	mat3 m;
	float materialId = MAT_BODY;
	vec3 posNose = pos;
	mat3 rot = rotz(pi2 * 0.5);
	rot *= rotx(pi2 * 0.75);

	float distort = (fbm(pos.xxz+offset)*0.2);

	//return vec2(10000.0, 0.0);

	// body
	float dist = sdSphere(pos-vec3(0.0, 20.0, 0.0)+distort+offset, vec4(20.0));
	//dist = min(sdTorus82(pos-vec3(18.4, 40.0, 37.0)+distort+offset, vec2(5.0)), dist);

	// head
	float head = sdSphere(pos-vec3(10.0, 55.0, 10.0)+distort+offset, vec4(10.0));
	dist = smin(dist, head, 2.0);
						
    // hat			
	float hat = sdTorus82((pos-vec3(20.0, 75.0, 20.0)+distort+offset)*rotx(0.0), vec2(4.0, 4.0)); // top 
	hat = smin(hat, sdTorus82((pos-vec3(20.0, 73.0, 20.0)+distort+offset)*rotx(0.0), vec2(10.0, 1.0)), 2.0); // ring
	if (hat<dist)
		materialId = MAT_HAT;
	dist = smin(dist, hat, 2.0);					

	// nose
	float nose = sdSphere(pos-vec3(18.0, 65.0, 28.0)+distort+offset, 2.0);
	if (nose<dist)
		materialId = MAT_NOSE;
	dist = smin(dist, nose, 2.0);

	// eyes
	float eyes = sdSphere(pos-vec3(15.0, 68.0, 27.0)+distort+offset, 1.0);	
	eyes = min(eyes, sdSphere(pos-vec3(22.0, 68.0, 27.0)+distort
	+offset, 1.0));
	if (eyes<dist)
		materialId = MAT_EYE;
	dist = smin(dist, eyes, 0.2);
	
	// arms
	//m = roty(pi2 * 0.25)*rotx(pi2 * 0.3);
	//float arms = sdCylinder((pos*m)-((vec3(-2.0, 0.0, 30.0)+offset)*m), vec2(1.0, 5.0));	
	//arms = smin( arms, sdCylinder((pos*m)-((vec3(-2.0, 0.0, 20.0)+offset)*m), vec2(1.0, 5.0)), 1.0);	
	
	/*
	m = roty(pi2 * -0.25)*rotx(pi2 * 0.3);
	arms = smin( arms, sdCylinder((pos*m)-((vec3( 40.0, 0.0, 30.0)+offset)*m), vec2(1.0, 5.0)), 1.0);	
	arms = smin( arms, sdCylinder((pos*m)-((vec3( 2.0, 0.0, 20.0)+offset)*m), vec2(1.0, 5.0)), 1.0);	

	arms = smin( arms, sdCylinder((pos-vec3(44.0, 48.5, 35.0)+offset)*rotz(pi2 * 0.21)*rotx(pi2 * 0.15), vec2(0.3, 2.0)), 1.0);	
	if (arms<dist)
		materialId = MAT_ARM;
	dist = smin(dist, arms, 3.6);
	*/
	// buttons

	float buttons = sdSphere(pos-vec3(18.0, 45.0, 36.5)+distort+offset, 1.5);
	
	buttons = min(sdSphere(pos-vec3(18.4, 40.0, 37.0)+distort+offset, 1.5), buttons);
	buttons = min(sdSphere(pos-vec3(18.0, 35.0, 37.5)+distort+offset, 1.5), buttons);
	buttons = min(sdSphere(pos-vec3(19.1, 50.0, 33.5)+distort+offset, 1.5), buttons);
	buttons = min(sdSphere(pos-vec3(18.7, 54.0, 31.0)+distort+offset, 1.5), buttons);
	
	if (buttons<dist)
		materialId = MAT_EYE;
	dist = smin(dist, buttons, 0.5);

	// mouth
	float mouth = 10000.0;
	mouth = min(sdSphere(pos-vec3(14.0, 63.0, 27.5)+distort+offset, 0.5), mouth);
	mouth = min(sdSphere(pos-vec3(16.0, 62.0, 28.5)+distort+offset, 0.5), mouth);
	mouth = min(sdSphere(pos-vec3(18.0, 61.0, 28.5)+distort+offset, 0.5), mouth);

	mouth = min(sdSphere(pos-vec3(20.0, 61.0, 28.5)+distort+offset, 0.5), mouth);
	mouth = min(sdSphere(pos-vec3(22.0, 62.0, 28.5)+distort+offset, 0.5), mouth);
	mouth = min(sdSphere(pos-vec3(24.0, 63.0, 28.0)+distort+offset, 0.5), mouth);
	
	if (mouth<dist)
		materialId = MAT_EYE;

	dist = smin(dist, mouth, 0.2);
			
	vec2 ret = vec2(dist, materialId);
	return ret;
}

vec2 map( in vec3 p, in bool masked )
{
    vec2 res = vec2(1000.0,-1.0); // dist, material

	//float box = sdBox(p-cube-vec3(2.0, 0.0, 0.0), vec3(1.0));

	// terrain
	float h = terrain( p.xz );
	float dd = (p.y - h);
	
	res = vec2( 1.0*dd, 0.0 );

	float dis = 100000.0;

    // trees	
	float crag = crags(p,h);
	//float tree = trees(p,h);
	//dis = min(dis, crag);
	res.x = min(dis, res.x);
	res.y = MAT_SNOW;

	//res.x = sdBox(p, vec3(10.0));

	if (masked)
	{
		#ifdef LOGO
		vec2 distDbf = distToDbf(p + vec3(objectPos.x, objectPos.y, objectPos.z));		
		if (distDbf.x < res.x)
		{
			//res.x = smin(distDbf.x, res.x, objectMix);
			res.y = distDbf.y;
		}
		res.x = smin(distDbf.x, res.x, objectMix);
		#endif
	
		#ifdef SNOWMAN
		vec2 distSnowman = distToSnowman(p, vec3(objectPos.x, objectPos.y, objectPos.z));		
		if (distSnowman.x < res.x)
		{
			res.y = distSnowman.y;
		}
		res.x = smin(distSnowman.x, res.x, objectMix);
		#endif
	}
	
	//res.y = MATERIAL_DBF - 0.1;
	/*
	if (box<res.x)
	{
		res.x = box;
		res.y = MAT_ARM;
	}*/
	return res;
}

vec3 intersect( in vec3 ro, in vec3 rd, in bool masked )
{
	float maxd = 500.0;
    float dist = precis*1.5;
    float t = 0.0;
	float d = 0.0;
    float m = 1.0;

	// march terrain
    for( int steps=0; steps<150; steps++ )
    {
        if( abs(dist)<precis||t>maxd )
			continue;//break;
        t += dist;

	    vec2 res = map( ro+rd*t, masked );
        dist = res.x;
		d = res.y; // distance
		m = res.y; // materialt6
    }

    if( t>maxd ) m=-1.0;
    return vec3( t, d, m );
}

vec3 calcNormal( in vec3 pos, in float prec, in bool masked )
{
    vec3 eps = vec3(prec ,0.0,0.0);

	return normalize( vec3(
           map(pos+eps.xyy, masked).x - map(pos-eps.xyy, masked).x,
           map(pos+eps.yxy, masked).x - map(pos-eps.yxy, masked).x,
           map(pos+eps.yyx, masked).x - map(pos-eps.yyx, masked).x ) );
}

vec3 calcNormal( in vec3 pos, in bool masked )
{
    vec3 eps = vec3(precis,0.0,0.0);

	return normalize( vec3(
           map(pos+eps.xyy, masked).x - map(pos-eps.xyy, masked).x,
           map(pos+eps.yxy, masked).x - map(pos-eps.yxy, masked).x,
           map(pos+eps.yyx, masked).x - map(pos-eps.yyx, masked).x ) );
}

float softshadow( in vec3 ro, in vec3 rd, in float k, in bool masked )
{
    float res = 1.0;
    float t = 0.0;
	float h = 1.0;
    for( int i=0; i<60; i++ )
    {
        h = map(ro + rd*t, masked).x;
        res = min( res, k*h/t );
		t += clamp( h, 0.02, 1.0 );
    }
    return clamp(res,0.0,1.0);
}

float calcOcc( in vec3 pos, in vec3 nor, in bool masked )
{
	float totao = 0.0;
    for( int aoi=0; aoi<8; aoi++ )
    {
        float hr = 0.1 + 1.5*pow(float(aoi)/8.0,2.0);
        vec3 aopos = pos + nor * hr;
        float dd = map( aopos, masked ).x;
        //totao += clamp( (hr-dd)*0.1-0.01,0.0,1.0);
		totao += max( 0.0, hr-3.0*dd-0.01);
    }
    return clamp( 1.0 - 0.15*totao, 0.0, 1.0 );
}

vec3 background(vec3 p,vec3 d)//render background
{
	vec3 color = mix(sky1 , sky2,d.y*.5+.5);
	return color*(noise(d)+.3*pow(noise(d*4.0),2.0));
	//return textureCube(iChannel0,d).xyz*vec3(.2,.4,.6);
}

void shade( in vec3 pos, in vec3 nor, in vec3 rd, in float matID, 
		    out vec3 bnor, out vec4 mate, out vec2 mate2 )
{
    bnor = vec3(0.0);
	mate = vec4(0.0);
	mate2 = vec2(0.0);

	vec3 snowTint = vec3(1.0, 1.0, 1.0);

	// Y C P
	if (matID == MAT_D)
	{
		snowTint = vec3(1.0, 1.0, 0.0);
	}else if (matID == MAT_B)
	{
		snowTint = vec3(0.0, 1.0, 0.0);
	}else if (matID == MAT_F)
	{
		snowTint = vec3(1.0, 0.0, 0.0);
	}
	
	if (matID == MAT_HAT)
	{
		mate.xyz = vec3(0.2, 0.3, 0.2);
		float iss = smoothstep( 0.2, 0.5, nor.y );
		//iss = mix( iss, 0.9, 0.75*smoothstep( 0.1, 1.0, texturize( iChannel1, 0.1*pos, nor ).x ) );
	
		vec3 snowColor = vec3( 0.65, 0.8, 0.8 );
	
		// specular reflections
		vec3 cnor = normalize( -1.0 + 2.0*texture2D( iChannel0, 0.65*pos.xz ).xyz );
		cnor.y = abs( cnor.y );
		float spe = max( 0.0, pow( clamp( dot(lig,reflect(rd, cnor)), 0.0, 1.0), 16.0 ) );
		mate2.y = spe*iss;
	
		mate.xyz = mix( mate.xyz, snowColor, iss );
	/*}else if (matID == MAT_BODY)
	{
		mate.xyz = vec3(0.8, 0.8, 0.8)*0.5;
		
		float iss = smoothstep( 0.2, 0.5, nor.y );
		//iss = mix( iss, 0.9, 0.75*smoothstep( 0.1, 1.0, texturize( iChannel1, 0.1*pos, nor ).x ) );
	
		vec3 snowColor = vec3( 0.65, 0.8, 0.8 );
	
		// specular reflections
		vec3 cnor = normalize( -1.0 + 2.0*texture2D( iChannel0, 0.65*pos.xz ).xyz );
		cnor.y = abs( cnor.y );
		float spe = max( 0.0, pow( clamp( dot(lig,reflect(rd, cnor)), 0.0, 1.0), 16.0 ) );
		mate2.y = spe*iss;
	
		mate.xyz = mix( mate.xyz, snowColor, iss );*/
	}else if (matID == MAT_NOSE)
	{
		mate.xyz = vec3(0.2, 0.175, 0.2);
		
		float iss = smoothstep( 0.2, 0.9, nor.y );
		iss = mix( iss, 0.2, 0.75*smoothstep( 0.9, 1.0, texturize( iChannel1, 0.1*pos, nor ).x ) );
	
		vec3 snowColor = vec3( 0.65, 0.8, 0.8 );
	
		// specular reflections
		vec3 cnor = normalize( -1.0 + 2.0*texture2D( iChannel0, 0.65*pos.xz ).xyz );
		cnor.y = abs( cnor.y );
		float spe = max( 0.0, pow( clamp( dot(lig,reflect(rd, cnor)), 0.0, 1.0), 16.0 ) );
		mate.y = spe*iss;
	
		mate.xyz = mix( mate.xyz, snowColor, iss );
	}else if (matID == MAT_EYE)
	{
		mate.xyz = vec3(0.1, 0.1, 0.1);
		
		float iss = smoothstep( 0.2, 0.9, nor.y );
		//iss = mix( iss, 0.9, 0.75*smoothstep( 0.1, 1.0, texturize( iChannel1, 0.1*pos, nor ).x ) );
	
		vec3 snowColor = vec3( 0.65, 0.8, 0.8 );
	
		// specular reflections
		vec3 cnor = normalize( -1.0 + 2.0*texture2D( iChannel0, 0.65*pos.xz ).xyz );
		cnor.y = abs( cnor.y );
		float spe = max( 0.0, pow( clamp( dot(lig,reflect(rd, cnor)), 0.0, 1.0), 16.0 ) );
		mate.y = spe*iss;
	
		mate.xyz = mix( mate.xyz, snowColor, iss*0.5 );
	
	}else if (matID == MAT_ARM)
	{
		mate.xyz = vec3(0.4, 0.25, 0.2);
		
		float iss = smoothstep( 0.2, 0.5, nor.y );
		//iss = mix( iss, 0.9, 0.75*smoothstep( 0.1, 1.0, texturize( iChannel1, 0.1*pos, nor ).x ) );
	
		vec3 snowColor = vec3( 0.65, 0.8, 0.8 );
	
		// specular reflections
		vec3 cnor = normalize( -1.0 + 2.0*texture2D( iChannel0, 0.65*pos.xz ).xyz );
		cnor.y = abs( cnor.y );
		float spe = max( 0.0, pow( clamp( dot(lig,reflect(rd, cnor)), 0.0, 1.0), 16.0 ) );
		mate.y = spe*iss;
	
		mate.xyz = mix( mate.xyz, snowColor, iss*0.5 );
	}else 
	{
		mate.xyz = vec3(0.1, 0.1, 0.1);
		
		float iss = smoothstep( 0.2, 0.5, nor.y );
		//iss = mix( iss, 0.9, 0.75*smoothstep( 0.1, 1.0, texturize( iChannel1, 0.1*pos, nor ).x ) );
	
		mate.xyz = vec3( 0.8, 0.8, 0.8 ) * snowTint;
	
		// specular reflections
		vec3 cnor = normalize( -1.0 + 2.0*texture2D( iChannel0, 0.65*pos.xz ).xyz );
		cnor.y = abs( cnor.y );
		float spe = max( 0.0, pow( clamp( dot(lig,reflect(rd, cnor)), 0.0, 1.0), 16.0 ) );
		mate.xyz = spe*iss;
	
		mate.xyz = mix( mate.xyz, vec3(0.8), iss );
		mate.xyz+=sky1*cnor.y*1;
		mate.xyz+=sky2*cnor.y*1;
	}
}

float cloudShadow( in vec3 pos )
{
	vec2 cuv = pos.xz + lig.xz*(100.0-pos.y)/lig.y;
	float cc = 0.1 + 0.9*smoothstep( 0.1, 0.35, texture2D( iChannel1, .0001*cuv + 0.1+.013*time ).x );
	
	return cc;
}

vec3 snow(vec3 ro, vec3 rd)
{
	return vec3(1.0, 1.0, 1.0);
}

void main(void)
{
	/*float hres = (g_resolution.y-(g_resolution.y / 1.75)) * 0.5;
	if (gl_FragCoord.y < hres || gl_FragCoord.y > g_resolution.y - hres)
	{
		gl_FragColor = vec4( vec3(0.0), 1.0 );
		return;
	}
	*/

	vec2 iMouse = vec2(0.0, 0.0);
	vec2 fp = gl_FragCoord.xy / g_resolution.xy;
	bool masked = texture2D(mask, fp).r > 0.001;
    //bool masked = true;
	vec2 xy = -1.0 + 2.0*fp;
	vec2 s = xy*vec2(1.75,1.0);
	float time = time*.15 + 2.0*iMouse.x/g_resolution.x;
	vec3 ro = cameraPosition;		
	vec3 rd = normalize(viewDirection);
	
	float ctime = time;

   //-----------------------------------------------------
	// render
    //-----------------------------------------------------

	// raymarch
    vec3 tmat = intersect(ro,rd, masked);

	vec3 d = vec3(xy, 1.0);
	//vec3 col = 2.5*vec3(0.18,0.10,0.10) - rd.y*0.5;
	//vec3 col = 2.5*vec3(0.10,0.10,0.10) - rd.y*0.5;
	//vec3 col = background(ro + tmat.x*rd, d);
	vec3 col = 2.5*background(ro + tmat.x*rd, d) - rd.y*1.5;
	//col *= 0.9;
    float sun = clamp( dot(rd, lig), 0.0, 1.0 );
    vec3 bgcol = col;

	float depth = 1.0;

    if( tmat.z>-0.5 )
    {
        // geometry
        vec3 pos = ro + tmat.x*rd;
        vec3 nor = tmat.z == MAT_SNOW ? calcNormal(pos, masked) : calcNormal(pos, masked);

        // materials
		vec4 mate = vec4(0.0);
		vec2 mate2 = vec2(0.0);
		vec3 bnor = vec3(0.0);
		shade( pos, nor, rd, tmat.z, bnor, mate, mate2 );
        nor = normalize( nor + bnor );
	
		vec3 ref = reflect( rd, nor );

		/*
		// lighting
		float occ = calcOcc(pos, nor, masked) * clamp(0.7 + 0.3*nor.y,0.0,1.0);
        float sky = 0.6 + 0.4*nor.y;
		float bou = clamp(-nor.y,0.0,1.0);
		float dif = max(dot(nor,lig),0.0);
        float bac = max(0.2 + 0.8*dot(nor,normalize(vec3(-lig.x,0.0,-lig.z))),0.0);
		float sha = 0.0; if( dif>0.01 ) sha=softshadow( pos+0.01*nor, lig, 64.0, masked );
		sha *= cloudShadow( pos );
        float fre = pow( clamp( 1.0 + dot(nor,rd), 0.0, 1.0 ), 3.0 );
		*/
		
		// lighting
		float occ = calcOcc(pos, nor, masked) * clamp(0.7 + 0.3*nor.y,0.0,1.0);
        float sky = 0.6 + 0.4*nor.y;
		float bou = clamp(-nor.y,0.0,1.0);
		float dif = max(dot(nor,lig),0.0);
        float bac = max(0.2 + 0.8*dot(nor,normalize(vec3(-lig.x,0.0,-lig.z))),0.0);
		float sha = 0.0; if( dif>0.01 ) sha=softshadow( pos+0.01*nor, lig, 64.0, masked );
		sha *= cloudShadow( pos );
        float fre = pow( clamp( 1.0 + dot(nor,rd), 0.0, 1.0 ), 3.0 );

		// lights
		vec3 lin = vec3(0.0);
		lin += 1.0*dif*vec3(1.70,1.15,0.70)*pow(vec3(sha),vec3(1.0,1.2,2.0));
		lin += 1.0*sky*vec3(0.25,0.20,0.25)*occ;
		//lin += 1.0*bac*vec3(0.20,0.25,0.25)*occ;
		lin += 1.2*bou*vec3(0.15,0.20,0.20)*(0.5+0.5*occ);
        lin += 1.0*fre*vec3(1.00,1.25,1.30)*occ*0.5*(0.5+0.5*dif*sha);
		lin += 1.0*mate2.y*vec3(1.00,0.60,0.50)*4.0*occ*dif*(0.1+0.9*sha);

		// surface-light interacion
		col = mate.xyz*lin;
		
		// fog
		col = mix( bgcol, col, exp(-0.0015*pow(tmat.x,1.0)) );
						
		vec4 worldHitPos = mviewproj * vec4( pos, 1.0 );
		float zc = worldHitPos.z;
		float wc = worldHitPos.w;		
		depth = zc/wc;		
		depth = (depth + 1.0)/2.0;		

	}
	
	// sun glow
    col += vec3(1.0,0.6,0.2)*0.4*pow( sun, 4.0 );


	//-----------------------------------------------------
	// postprocessing
    //-----------------------------------------------------
    // gamma
	col = pow( clamp(col,0.0,1.0), vec3(0.45) );
	gl_FragDepth = depth;
	gl_FragColor = vec4( vec3(col), 1.0 );
}
