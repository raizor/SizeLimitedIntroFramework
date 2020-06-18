/********************************************************************
** STITCH SOURCE: 'ss.version330' **
********************************************************************/
#version 330
/********************************************************************
** STITCH SOURCE: 'ss.simplex4d' **
********************************************************************/
//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110410 (stegu)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//

vec4 permute(vec4 x)
{
  return mod(((x*34.0)+1.0)*x, 289.0);
}

float permute(float x)
{
  return floor(mod(((x*34.0)+1.0)*x, 289.0));
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float taylorInvSqrt(float r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec4 grad4(float j, vec4 ip)
  {
  const vec4 ones = vec4(1.0, 1.0, 1.0, -1.0);
  vec4 p,s;

  p.xyz = floor( fract (vec3(j) * ip.xyz) * 7.0) * ip.z - 1.0;
  p.w = 1.5 - dot(abs(p.xyz), ones.xyz);
  s = vec4(lessThan(p, vec4(0.0)));
  p.xyz = p.xyz + (s.xyz*2.0 - 1.0) * s.www; 

  return p;
  }

float snoise(vec4 v)
  {
  const vec4  C = vec4( 0.138196601125011,  // (5 - sqrt(5))/20  G4
                        0.276393202250021,  // 2 * G4
                        0.414589803375032,  // 3 * G4
                       -0.447213595499958); // -1 + 4 * G4
						
// (sqrt(5) - 1)/4 = F4, used once below
#define F4 0.309016994374947451

// First corner
  vec4 i  = floor(v + dot(v, vec4(F4)) );
  vec4 x0 = v -   i + dot(i, C.xxxx);

// Other corners

// Rank sorting originally contributed by Bill Licea-Kane, AMD (formerly ATI)
  vec4 i0;
  vec3 isX = step( x0.yzw, x0.xxx );
  vec3 isYZ = step( x0.zww, x0.yyz );
//  i0.x = dot( isX, vec3( 1.0 ) );
  i0.x = isX.x + isX.y + isX.z;
  i0.yzw = 1.0 - isX;
//  i0.y += dot( isYZ.xy, vec2( 1.0 ) );
  i0.y += isYZ.x + isYZ.y;
  i0.zw += 1.0 - isYZ.xy;
  i0.z += isYZ.z;
  i0.w += 1.0 - isYZ.z;

  // i0 now contains the unique values 0,1,2,3 in each channel
  vec4 i3 = clamp( i0, 0.0, 1.0 );
  vec4 i2 = clamp( i0-1.0, 0.0, 1.0 );
  vec4 i1 = clamp( i0-2.0, 0.0, 1.0 );

  //  x0 = x0 - 0.0 + 0.0 * C.xxxx
  //  x1 = x0 - i1  + 0.0 * C.xxxx
  //  x2 = x0 - i2  + 0.0 * C.xxxx
  //  x3 = x0 - i3  + 0.0 * C.xxxx
  //  x4 = x0 - 1.0 + 4.0 * C.xxxx
  vec4 x1 = x0 - i1 + C.xxxx;
  vec4 x2 = x0 - i2 + C.yyyy;
  vec4 x3 = x0 - i3 + C.zzzz;
  vec4 x4 = x0 + C.wwww;

// Permutations
  i = mod(i, 289.0); 
  float j0 = permute( permute( permute( permute(i.w) + i.z) + i.y) + i.x);
  vec4 j1 = permute( permute( permute( permute (
             i.w + vec4(i1.w, i2.w, i3.w, 1.0 ))
           + i.z + vec4(i1.z, i2.z, i3.z, 1.0 ))
           + i.y + vec4(i1.y, i2.y, i3.y, 1.0 ))
           + i.x + vec4(i1.x, i2.x, i3.x, 1.0 ));

// Gradients: 7x7x6 points over a cube, mapped onto a 4-cross polytope
// 7*7*6 = 294, which is close to the ring size 17*17 = 289.
  vec4 ip = vec4(1.0/294.0, 1.0/49.0, 1.0/7.0, 0.0) ;

  vec4 p0 = grad4(j0,   ip);
  vec4 p1 = grad4(j1.x, ip);
  vec4 p2 = grad4(j1.y, ip);
  vec4 p3 = grad4(j1.z, ip);
  vec4 p4 = grad4(j1.w, ip);

// Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;
  p4 *= taylorInvSqrt(dot(p4,p4));

// Mix contributions from the five corners
  vec3 m0 = max(0.6 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.0);
  vec2 m1 = max(0.6 - vec2(dot(x3,x3), dot(x4,x4)            ), 0.0);
  m0 = m0 * m0;
  m1 = m1 * m1;
  return 49.0 * ( dot(m0*m0, vec3( dot( p0, x0 ), dot( p1, x1 ), dot( p2, x2 )))
               + dot(m1*m1, vec2( dot( p3, x3 ), dot( p4, x4 ) ) ) ) ;

  }

vec3 fbm3(vec3 p, float time, float numOcts, float freq, float gain, float lacunarity, float roughness)
{
	vec3 v = vec3(0.0);

	float f = freq;
	float g = gain;

	for (float oct = 1.0; oct <= numOcts; oct += 1.0)
	{
		v += g * vec3(
			snoise(vec4(p * f + vec3(4.0, 1.2, 0.5 + oct), time)),
			snoise(vec4(p * f + vec3(6.1, 4.8 + oct, 7.3), time)),
			snoise(vec4(p * f + vec3(3.7 + oct, 7.4, 3.7), time))
		);

		f *= lacunarity;
		g *= roughness;
	}

	return v;
}

/*******************************************************************/
// geom shader for tunnel

uniform mat4 projectionMatrix;
uniform sampler2D input0;

//[
uniform int iGridSize;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform float fCurveZ;
uniform float fRadialBlockDisplaceAmount;
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;


uniform float fSeed1;
uniform float fSeed2;

uniform float fExtrude;

uniform float fTime;
uniform float fPrecalcPercentage;

uniform int iIsPrecalc;

uniform float fRadius;
uniform int iScene;
//]

float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float randGaussian(vec2 co)
{
	float r1 = rand(co + vec2(71.3, 56.4));
	float r2 = rand(co + vec2(32.9, 28.5));

	return 0.03 + sqrt(-2.0*log(r1*0.999+0.001))*cos(6.283185*r2);
}


mat4 mrotx(float angle)
{
	float c = cos(angle),s = sin(angle);

	return mat4(
		1, 0, 0, 0,
		0, c, s, 0,
		0, -s, c, 0,
		0, 0, 0, 1);
}

mat4 mroty(float angle)
{
	float c = cos(angle),s = sin(angle);

	return mat4(
		c, 0, -s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1);
}

mat4 mrotz(float angle)
{
	float c = cos(angle),s = sin(angle);

	return mat4(
		c, s, 0, 0,
		-s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

mat4 mscale(float x, float y, float z)
{
	return mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
}

mat4 mtranslate(float x, float y, float z)
{
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1);
}

vec4 vertices[8];

out vec3 viewspaceNormal;
out vec4 viewspacePosition4;
out vec3 viewspacePosition3;

void emitface(int i, int j, int k, int l, vec4 normal)
{
	viewspaceNormal = normalize(normal.xyz);

	viewspacePosition4 = vertices[i];
	viewspacePosition3 = vertices[i].xyz;
	gl_Position = projectionMatrix*vertices[i];
	EmitVertex();

	viewspacePosition4 = vertices[j];
	viewspacePosition3 = vertices[j].xyz;
	gl_Position = projectionMatrix*vertices[j];
	EmitVertex();

	viewspacePosition4 = vertices[k];
	viewspacePosition3 = vertices[k].xyz;
	gl_Position = projectionMatrix*vertices[k];
	EmitVertex();

	viewspacePosition4 = vertices[l];
	viewspacePosition3 = vertices[l].xyz;
	gl_Position = projectionMatrix*vertices[l];
	EmitVertex();

	EndPrimitive();
}

void emitcube(mat4 m)
{
	mat4 vm = modelViewMatrix * m;

	vertices[0] = vm*vec4(1, 1, 1, 1);
	vertices[1] = vm*vec4(-1, 1, 1, 1);
	vertices[2] = vm*vec4(-1, -1, 1, 1);
	vertices[3] = vm*vec4(1, -1, 1, 1);
	vertices[4] = vm*vec4(1, 1, -1, 1);
	vertices[5] = vm*vec4(-1, 1, -1, 1);
	vertices[6] = vm*vec4(-1, -1, -1, 1);
	vertices[7] = vm*vec4(1, -1, -1, 1);

	emitface(0, 1, 3, 2, vm*vec4(0, 0, 1, 0)); // back
	emitface(5, 4, 6, 7, vm*vec4(0, 0, -1, 0)); // front
	emitface(4, 0, 7, 3, vm*vec4(1, 0, 0, 0)); // right
	emitface(1, 5, 2, 6, vm*vec4(-1, 0, 0, 0)); // left
	emitface(1, 0, 5, 4, vm*vec4(1, 0, 0, 0)); // top
	emitface(3, 2, 7, 6, vm*vec4(-1, 0, 0, 0)); // bottom
}

void main()
{
	vec4 p = gl_in[0].gl_Position;
	vec2 pi = floor(p.xy * iGridSize);
	
	float resu = 160, resv = 240;
	float u = pi.x, v = pi.y;

	float i = resu*v + u;
	float resi = resu*resv;
	
	//mat4 t = mtranslate(0.0, 1.0 * 5.5 * u, 0.0);
	//t *= mroty(fTime) * mrotx(fTime * 0.2f);
	 mat4 t = mtranslate(p.x, p.y, 0.0);
	emitcube(t);
	
}
