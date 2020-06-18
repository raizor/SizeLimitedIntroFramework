// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

//stereo thanks to Croqueteer
//#define STEREO 


varying vec2 uv;
uniform float iGlobalTime;

varying vec3 cameraPosition;

varying mat4 mview;

uniform float znear;
uniform float zfar;

uniform mat4 mvm;
uniform mat4 imvm;
varying mat4 pmat;

varying float nearPlanePosition;
varying vec3 viewDirection;
uniform sampler2D iChannel0;          // input channel. XX = 2D/Cube


vec3 light1 = normalize( vec3(  -1.4, 10.52,  5.0) );

// Camera uniforms
vec2 g_resolution = vec2(800.0, 600.0);

float hash( float n )
{
    return fract(sin(n)*(43758.5453120));
}

int noiseMode = 0;

float sdSphere( vec3 p, float s )
{
  return length(p)-s;
}


float noise3( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);
		
	if (noiseMode == 0)
	{
		vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
		vec2 rg = texture2D( iChannel0, (uv+ 0.5)/256.0, -100.0 ).yx;
		return mix( rg.x, rg.y, f.z );	
		//return texture2D( iChannel0, (uv+ 0.5)/256.0, -100.0 ).x;
	}else{
		//vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
		//return texture3D(iChannel3d, vec3((uv+ 0.5)/256.0, f.z), -100.0).x;
	}
}

vec3 noised( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);

    vec2 u = f*f*(3.0-2.0*f);

    float n = p.x + p.y*2.0;

    float a = hash(n+  0.0);
    float b = hash(n+  1.0);
    float c = hash(n+ 2.0);
    float d = hash(n+ 3.0);


	return vec3(a+(b-a)*u.x+(c-a)*u.y+(a-b-c+d)*u.x*u.y,
				30.0*f*f*(f*(f-2.0)+1.0)*(vec2(b-a,c-a)+(a-b-c+d)*u.yx));

}

float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0;

    float res = mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                    mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);

    return res;
}

const mat2 m2 = mat2(1.6,-1.2,1.2,1.6);
	
float fbm( vec2 p )
{
    float f = 0.0;

    f += 0.5000*noise( p ); p = m2*p*2.02;
    f += 0.2500*noise( p ); p = m2*p*2.03;
    f += 0.1250*noise( p ); p = m2*p*2.01;
    f += 0.0625*noise( p );

    return f/0.9375;
}


float envelope( vec3 p )
{
	float isLake = 1.0-smoothstep( 0.62, 0.72, texture2D( iChannel0, 0.001*p.zx,-100.0).x );
	return 0.1 + isLake*0.9*texture2D( iChannel0, 0.01*p.xz ).x;
}

/*
vec4 mapTrees( in vec3 pos, in vec3 rd, float tmax)
{
    vec3  col = vec3(0.0);	
	float den = 1.0;

	float kklake = texture2D( iChannel0, 0.001*pos.zx,-100.0).x;
	float isLake = smoothstep( 0.7, 0.71, kklake );
	
	if( pos.y>1.0 || pos.y<0.0 ) 
	{
		den = 0.0;
	}
	else
	{
		
		float h = pos.y;
		float e = envelope( pos );
		float r = clamp(h/e,0.0,1.0);
		
        den = smoothstep( r, 1.0, texture2D(iChannel0, pos.xz*0.15, -100.0).x );
        
		den *= 1.0-0.95*clamp( (r-0.75)/(1.0-0.75) ,0.0,1.0);
		
        float id = texture2D( iChannel0, pos.xz,-100.0).x;
        float oc = pow( r, 2.0 );

		vec3  nor = calcNormal( pos, tmax );
		vec3  dif = vec3(1.0)*clamp( dot( nor, light1 ), 0.0, 1.0 );
		float amb = 0.5 + 0.5*nor.y;
		
		float w = (2.8-pos.y)/light1.y;
		float c = fbm( (pos+w*light1)*0.35 );
		c = smoothstep( 0.38, 0.6, c );
		dif *= pow( vec3(c), vec3(0.8, 1.0, 1.5 ) );
			
		vec3  brdf = 1.7*vec3(1.5,1.0,0.8)*dif*(0.1+0.9*oc) + 1.3*amb*vec3(0.1,0.15,0.2)*oc;

		vec3 mate = 0.6*vec3(0.5,0.5,0.1);
		mate += 0.3*texture2D( iChannel0, 0.1*pos.xz ).zyx;
		
		col = brdf * mate;

		den *= 1.0-isLake;
	}

	return vec4( col, den );
}
*/
/**
vec4 raymarchTrees( in vec3 ro, in vec3 rd, float tmax, vec3 bgcol )
{
	vec4 sum = vec4(0.0);
    float t = tmax;
	for( int i=0; i<512; i++ )
	{
		vec3 pos = ro + t*rd;
		if( sum.a>0.99 || pos.y<0.0 ) break;
		
		vec4 col = mapTrees( pos, rd, tmax );

		col.xyz = mix( col.xyz, bgcol, 1.0-exp(-0.0018*t*t) );
        
		col.rgb *= col.a;

		sum = sum + col*(1.0 - sum.a);	
		
		t += 0.0035*t;
	}

	sum.xyz /= (0.001+sum.w);

	return clamp( sum, 0.0, 1.0 );
}*/


float terrain( in vec2 x )
{
	vec2  p = x*0.003;
    float a = 0.0;
    float b = 1.0;
	vec2  d = vec2(0.0);
    for(int i=0;i<5; i++)
    {
        vec3 n = noised(p);
        d += n.yz;
        a += b*n.x/(1.0+dot(d,d));
		b *= 0.5;
        p = m2*p;
    }

    return 140.0*a;
}

float terrain2( in vec2 x )
{
	vec2  p = x*0.003;
    float a = 0.0;
    float b = 1.0;
	vec2  d = vec2(0.0);
    for(int i=0;i<14; i++)
    {
        vec3 n = noised(p);
        d += n.yz;
        a += b*n.x/(1.0+dot(d,d));
		b *= 0.5;
        p=m2*p;
    }

    return 140.0*a;
}

float map( in vec3 p )
{
	float h = terrain(p.xz);
	/*
	float ss = 0.03;
	float hh = h*ss;
	float fh = fract(hh);
	float ih = floor(hh);
	fh = mix( sqrt(fh), fh, smoothstep(50.0,140.0,h) );
	h = (ih+fh)/ss;
	*/
    return p.y - h;
}

float map2( in vec3 p )
{
	float h = terrain2(p.xz);

	/*
	float ss = 0.03;
	float hh = h*ss;
	float fh = fract(hh);
	float ih = floor(hh);
	fh = mix( sqrt(fh), fh, smoothstep(50.0,140.0,h) );
	h = (ih+fh)/ss;
	*/
    return p.y - h;
}



vec3 calcNormal( in vec3 pos, float t )
{
	float e = 0.001;
	e = 0.001*t;
    vec3  eps = vec3(e,0.0,0.0);
    vec3 nor;
    nor.x = map2(pos+eps.xyy) - map2(pos-eps.xyy);
    nor.y = map2(pos+eps.yxy) - map2(pos-eps.yxy);
    nor.z = map2(pos+eps.yyx) - map2(pos-eps.yyx);
    return normalize(nor);
}

bool marchObjects()
{
	return true;
}


float terrainHeightForEye( in vec2 x )
{
	vec2  p = x*0.003;
    float a = 0.0;
    float b = 1.0;
	vec2  d = vec2(0.0);
    for(int i=0;i<8; i++)
    {
        vec3 n = noised(p);
        d += n.yz;
        a += b*n.x/(1.0+dot(d,d));
		b *= 0.5;
        p = m2*p;
    }

    return 140.0*a;
}




#define OBJ_SKY 0
#define OBJ_IGLOO 1
#define OBJ_TERRAIN 2

int marchTerrain(in vec3 rO, in vec3 rD, out float distanceTravelled)
{
    float h = 0.0;
    float t = 0.0;
	for( int j=0; j<128; j++ )
	{
        //if( t>2000.0 ) break;
		
	    vec3 p = rO + t*rD;

        if( p.y>300.0 ) break;
		
        h = map( p );
		
		if( h<0.01 )
		{
			distanceTravelled = t; 
			return OBJ_TERRAIN;
		}
		t += max(0.1,0.5*h);

	}

	if( h<10.0 )
    {
	    distanceTravelled = t;
	    return true;
	}
	return OBJ_SKY;
}

#define MAX_STEPS 128

#define CHAR_W 5.0
#define CHAR_H 25.0

// polynomial smooth min (k = 0.1);
float smin( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

float udRoundBox( vec3 pos, vec3 size, float r )
{
  return length(max(abs(pos)-size,0.0))-r;
}

float sdBox( vec3 pos, vec3 size )
{
//return udRoundBox(pos, size, 1.0);
  vec3 d = abs(pos) - size;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}

float sdBoxCentered( vec3 pos, vec3 size )
{
	pos.x -= size.x;
	pos.y += size.y;
	pos.z += size.z;

	//

  //pos+=size*0.5;
  vec3 d = abs(pos) - size;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}

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


float char_d(in vec3 pos)
{
	float height = 5.0 * 7.0;

	float mina = 0.0;
	float dist = sdBoxCentered(vec3(0.0, 0.0, 0.0) - pos, vec3(5.0, height, 5.0)); // left
	float dist2 = sdBoxCentered(vec3(height-5.0, 0.0, 0.0) - pos, vec3(5.0, height, 5.0)); // right
	dist = smin(dist, dist2, mina);
	
	dist2 = sdBoxCentered(vec3(0.0, 0.0, 0.0) - pos, vec3(5.0, height, 5.0));  // mid
	//dist = smin(dist, dist2, mina);

	return dist;
}

#define CHAR_A 1
#define CHAR_B 2
#define CHAR_C 3
#define CHAR_D 4

float doChar(in vec3 pos, int charCode)
{
	float dist = 0.0;
	float dist2 = 0.0;
	float mina = 0.0;

	if (charCode == CHAR_A)
	{
		dist = sdBoxCentered(pos - vec3(0.0, 25.0, 0.0), vec3(5.0, 5.0 * 7.0, 5.0)); // left
		dist2 = sdBoxCentered(pos - vec3(5.0 * 6.0, 25.0, 0.0), vec3(5.0, 5.0 * 7.0, 5.0)); // right
		dist = smin(dist, dist2, mina);
	
		dist2 = sdBoxCentered(pos - vec3(0.0, 25.0, 0.0), vec3(15.0, 5.0, 5.0)); // top
		dist = smin(dist, dist2, mina);

		dist2 = sdBoxCentered(pos - vec3(0.0, -5.0, 0.0), vec3(20.0, 5.0, 5.0)); // mid
		dist = smin(dist, dist2, mina);

		//dist2 = sdBoxCentered(pos - vec3(0.0, -35.0, 0.0), vec3(20.0, 5.0, 5.0)); // bot
		//dist = smin(dist, dist2, mina);
	}else if (charCode == CHAR_B)
	{
		dist = sdBoxCentered(pos - vec3(0.0, 25.0, 0.0), vec3(5.0, 5.0 * 7.0, 5.0)); // left
		
		dist2 = sdBoxCentered(pos - vec3(20.0, 25.0, 0.0), vec3(5.0, 15.0, 5.0)); // right / top
		dist = smin(dist, dist2, 0.0);
		
		dist2 = sdBoxCentered(pos - vec3(30.0, -15.0, 0.0), vec3(5.0, 15.0, 5.0)); // right / bot
		dist = smin(dist, dist2, 0.0);

	
		dist2 = sdBoxCentered(pos - vec3(0.0, 25.0, 0.0), vec3(15.0, 5.0, 5.0)); // top
		dist = smin(dist, dist2, mina);

		dist2 = sdBoxCentered(pos - vec3(0.0, -5.0, 0.0), vec3(20.0, 5.0, 5.0)); // mid
		dist = smin(dist, dist2, mina);

		dist2 = sdBoxCentered(pos - vec3(0.0, -35.0, 0.0), vec3(20.0, 5.0, 5.0)); // bot
		dist = smin(dist, dist2, mina);
	}else if (charCode == CHAR_C)
	{
		dist = sdBoxCentered(pos - vec3(0.0, 25.0, 0.0), vec3(5.0, 5.0 * 7.0, 5.0)); // left
		//dist2 = sdBoxCentered(pos - vec3(5.0 * 6.0, 25.0, 0.0), vec3(5.0, 5.0 * 7.0, 5.0)); // right
		//dist = smin(dist, dist2, mina);
	
		dist2 = sdBoxCentered(pos - vec3(0.0, 25.0, 0.0), vec3(15.0, 5.0, 5.0)); // top
		dist = smin(dist, dist2, mina);

		//dist2 = sdBoxCentered(pos - vec3(0.0, -5.0, 0.0), vec3(20.0, 5.0, 5.0)); // mid
		//dist = smin(dist, dist2, mina);

		dist2 = sdBoxCentered(pos - vec3(0.0, -35.0, 0.0), vec3(20.0, 5.0, 5.0)); // bot
		dist = smin(dist, dist2, mina);
	}else if (charCode == CHAR_D)
	{
		dist = sdBoxCentered(pos - vec3(0.0, 25.0, 0.0), vec3(5.0, 5.0 * 7.0, 5.0)); // left
		
		dist2 = sdBoxCentered(pos - vec3(20.0, 25.0, 0.0), vec3(5.0, 15.0, 5.0)); // right / top
		dist = smin(dist, dist2, 0.0);
		
		dist2 = sdBoxCentered(pos - vec3(30.0, -15.0, 0.0), vec3(5.0, 15.0, 5.0)); // right / bot
		dist = smin(dist, dist2, 0.0);

	
		dist2 = sdBoxCentered(pos - vec3(0.0, 25.0, 0.0), vec3(15.0, 5.0, 5.0)); // top
		dist = smin(dist, dist2, mina);

		dist2 = sdBoxCentered(pos - vec3(20.0, -5.0, 0.0), vec3(10.0, 5.0, 5.0)); // mid
		dist = smin(dist, dist2, mina);

		dist2 = sdBoxCentered(pos - vec3(0.0, -35.0, 0.0), vec3(20.0, 5.0, 5.0)); // bot
		dist = smin(dist, dist2, mina);
	}
	return dist;
}

#define SX	245.0
#define YX	168.0
#define MX	105.0
#define BX	-25.0
#define IX	-135.0
#define OX	-215.0

float ring(in vec3 p, in vec2 r)
{
	return abs(length(p.xy)-r.x)-r.y;
}

float box(in vec3 p, in vec3 r)
{
	vec3 d = abs(p)-r;
	return max(d.x,d.y);
}

float sub(in float d1, in float d2)
{
	return max(d1,-d2);
}

vec2 opUnion( vec2 d1, vec2 d2 )
{
	return (d1.x<d2.x) ? d1 : d2;
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

float opSub( float d1, float d2 )
{
    return max(-d2,d1);
}

vec3 opRep( vec3 p, vec3 c )
{
    return mod(p,c)-0.5*c;
}

float sdTorus82( vec3 p, vec2 t )
{
  vec2 q = vec2(length2(p.xz)-t.x,p.y);
  return length8(q)-t.y;
}

float sdCylinder( vec3 p, vec2 h )
{
  return max( length6(p.xz)-h.x, abs(p.y)-h.y );
}

#define pi2 6.28318530718

float distToCog(in vec3 pos)
{
	////float cog = udRoundBox(pos+vec3(10.0, 1.0, 0.0), vec3(2.20, 10.0, 2.2), 1.0);
	//return cog; 
	
	
    return vec2( opSub(
		             sdTorus82(  pos-vec3(-2.0,0.2, 0.0), vec2(0.20,0.1)),
	                 sdCylinder(  opRep( vec3(atan(pos.x+2.0,pos.z)/6.2831 + 0.1,
											  pos.y,
											  0.02+0.5*length(pos-vec3(-2.0,0.2, 0.0))),
									     vec3(0.05,1.0,0.05)), vec2(0.02,0.6))), 51.0 );

	
}

float distToDbf(in vec3 pos)
{
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
	
	return min(min(f, d),b);
}

float distToScene(in vec3 pos)
{
	float dbf = distToDbf(pos);
	float cog = distToCog(pos);
	return min(dbf, cog);
}

vec3 calcNormalObj( in vec3 pos, float t )
{
	float e = 0.001;
	e = 0.001*t;
    vec3  eps = vec3(e,0.0,0.0);
    vec3 nor;
    nor.x = distToScene(pos+eps.xyy) - distToScene(pos-eps.xyy);
    nor.y = distToScene(pos+eps.yxy) - distToScene(pos-eps.yxy);
    nor.z = distToScene(pos+eps.yyx) - distToScene(pos-eps.yyx);
    return normalize(nor);
}


int marchScene(in vec3 rO, in vec3 rD, out float distanceTravelled, out float snowAmount)
{
	float t = 0.0;

	snowAmount = 0.0;

	for( int j=0; j<MAX_STEPS; j++ )
	{
	    vec3 p = rO + t*rD;
        if( p.y>300.0 ) break;

		float distScene = distToScene(p);
		float distTerrain = map( p );	

		float dist = smin(distScene, distTerrain, 6.1);

		distanceTravelled = t;		

		if( dist<0.01)
		{	
			snowAmount = clamp(1.5 - distTerrain * (fbm(p.xz) ), 0.0, 1.0);
			return OBJ_TERRAIN;
		}
		t += dist;
	}
	return OBJ_SKY;
}


void main(void)
{
	vec2 iMouse = vec2(0.0);

    vec2 xy = -1.0 + 2.0*gl_FragCoord.xy / g_resolution.xy;

	vec2 s = xy*vec2(1.75,1.0);
	
    float time = iGlobalTime*.15 + 2.0*iMouse.x/g_resolution.x;

	vec3 ro = cameraPosition;	
	ro.z *= iGlobalTime;
	ro.y = terrainHeightForEye( ro.xz ) + 9.0;
	
	vec3 rd = normalize(viewDirection);
	
	float sundot = clamp(dot(rd,light1),0.0,1.0);
	vec3 col;
    float t;

	gl_FragDepth = 1;

	float height = 0.0;

	float snowAmount = 0.0;

	int objectTypeHit = marchScene(ro, rd, t, snowAmount);

	vec3 bcol = vec3(0.5);

	float gt = (0.0-ro.y)/rd.y;

    if( objectTypeHit == OBJ_SKY )
    {
		// sky		
		col = vec3(0.5,.6,0.7)*(1.0-0.5*rd.y);
		
		col += 0.25*vec3(1.0,0.8,0.5)*pow( sundot, 5.0 );

		//float zc = ( pmat * vec4( p, 1.0 ) ).z;
		//float wc = ( pmat * vec4( p, 1.0 ) ).w;
		//gl_FragDepth = 1;//zc/wc;
	}
	else
	{
		
		float s = 1.0;
		vec3 pos = ro + t*rd;
		
		// lighting

		// snow
		vec3 normalSnow = calcNormal( pos, t );
		vec3 snowCol = 0.35*vec3(0.5,0.5,0.7);
		float difSnow = clamp( dot( light1, normalSnow ), 0.0, 1.0 );
		float ambSnow = clamp(0.5+0.5*normalSnow.y,0.0,1.0);		
		float bacSnow = clamp( 0.2 + 0.8*dot( normalize( vec3(-light1.x, 0.0, light1.z ) ), normalSnow ), 0.0, 1.0 );
				
		vec3 linSnow  = vec3(0.0);
		linSnow += difSnow*vec3(6.60,5.10,3.30);
		linSnow += ambSnow*vec3(0.38,0.55,0.63);
		linSnow += bacSnow*vec3(0.60,0.60,0.60);

		snowCol *= linSnow;

		// objects
		vec3 normalObjects = calcNormalObj( pos, t );
		vec3 objCol = 0.35*vec3(0.4,0.45,0.5);
		float difObjects = clamp( dot( light1, normalObjects ), 0.0, 1.0 );
		float ambObjects = clamp(0.5+0.5*normalObjects.y,0.0,1.0);		
		float bacObjects = clamp( 0.2 + 0.8*dot( normalize( vec3(-light1.x, 0.0, light1.z ) ), normalObjects ), 0.0, 1.0 );
				
		vec3 linObjects  = vec3(0.0);
		linObjects += difObjects*vec3(6.60,5.10,3.30);
		linObjects += ambObjects*vec3(0.38,0.55,0.63);
		linObjects += bacObjects*vec3(0.60,0.60,0.60);

		objCol *= linObjects;


		col = mix(objCol, snowCol, snowAmount);

	}

	col = pow(col,vec3(0.45));

	col = col*0.66 + 0.34*col*col*(3.0-2.0*col);
	
	vec2 uv = xy*0.5+0.5;
	col *= 0.7 + 0.3*pow(16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y),0.1);
		
	gl_FragColor=vec4(col,1.0);
}
