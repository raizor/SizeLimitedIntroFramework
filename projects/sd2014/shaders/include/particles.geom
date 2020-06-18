/********************************************************************
** STITCH SOURCE: 'ss.version330' **
********************************************************************/
#version 330
/********************************************************************
** STITCH SOURCE: 'ss.random' **
********************************************************************/
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

vec3 gauss3(vec2 p, float seed)
{
	return vec3(randGaussian(p + vec2(19.412 + seed*0.981, 11.183+seed*1.5677)), randGaussian(p + vec2(86.287+seed*4.6671, 87.242+seed*8.6112)), randGaussian(p + vec2(37.563+seed*3.367, 23.543+seed*5.729)));
}
/*******************************************************************/
//uniform mat4 modelViewProjectionMatrix;

#define pi2 6.28318530718
//[
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
//]
out vec2 quadCoord;
out float quadSize;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform float fTime;
uniform vec3 eye;
uniform vec3 rot;
//uniform mat4 viewMatrix;
//uniform mat4 inverseViewMatrix;

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

#define DEG_PER_RAD 0.0174532925

void main(void)
{
	vec3 o = vec3(0.05, 0.0, -0.05)*0.5;

	vec4 p = gl_in[0].gl_Position;

	vec3 r = vec3(
		rand(p.xy + vec2(23.098, 47.863)),
		rand(p.xy + vec2(98.672, 38.942)),
		rand(p.xy + vec2(86.573, 24.182))
	);
				
	vec4 downVec = vec4(0.0, 1.0, -0.75, 1.0);

	downVec.xyz += r * p.xyz;

	p.xyz = r;
	
	mat3 rotMat = rotx(pi2 * (rot.x / 360.0));
	rotMat *= roty(pi2 * (rot.y / 360.0));
	rotMat *= rotz(pi2 * (rot.z / 360.0));
	
	//p *= modelViewMatrix;	
	p.xyz+=eye * 0.001;
	p.xyz -= (fTime + 100.0) * 0.05 * downVec.xyz;
	
	p.xyz = mod(p.xyz, 1.0);				

	p.xyz -= 0.5;
	p.xyz *= 16.0;	

	p.xyz *= rotMat;

	p.w = 1.0;

	float size = 0.4;
	o *= size;

	if (size > 0.1)
	{
		quadSize = o.x * 50.0;

		gl_Position = projectionMatrix*(p + o.xzyy);
		quadCoord = vec2(1.0, -1.0);
		EmitVertex();

		gl_Position = projectionMatrix*(p + o.xxyy);
		quadCoord = vec2(1.0, 1.0);
		EmitVertex();

		gl_Position = projectionMatrix*(p + o.zzyy);
		quadCoord = vec2(-1.0, -1.0);
		EmitVertex();

		gl_Position = projectionMatrix*(p + o.zxyy);
		quadCoord = vec2(-1.0, 1.0);
		EmitVertex();
	}
}
