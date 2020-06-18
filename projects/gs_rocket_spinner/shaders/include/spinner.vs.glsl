#version 330

uniform float fArmMult;
//[
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 index;

out CubeData
{
	vec2 index;
    float time;
    float scale;
    float xrot;
    float yrot;
    vec3 offset;    
}vertex;
//]
#define HPI 1.57079
#define PI 3.141592
#define PI2 6.283185

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

void main(void)
{	
	float armMult = PI2 / fArmMult;
	float gridSize = 256.0;
	gl_Position = position;
	vertex.index = index;
	
	//vec4
	
	float r1 = randGaussian(position.xy);
	float r2 = randGaussian(position.xy * r1);
	float r3 = randGaussian(position.xy * r2);
	float r4 = randGaussian(position.xy * r3);
	
	vec4 gaus = vec4(r1, r2, r3, r4);
	
	vertex.time = gaus.x * 50.0 + 50.0;
	vertex.scale = gaus.y * .2 + .3;
	vertex.xrot  = floor(gaus.z * 2.0 + 2.0) * armMult;
	vertex.yrot  = floor(gaus.w * 2.0 + 2.0) * armMult;
	vertex.offset = gaus.xyz * 0.4;
	
	//vertex
	

/*
vertex.time = texture2D(input0, index / gridSize).TIME;
	vertex.scale = texture2D(input0, index / gridSize).SCALE;
	vertex.xrot = texture2D(input0, index / gridSize).XROT;
	vertex.yrot = texture2D(input0, index / gridSize).YROT;
	vertex.offset = texture2D(input1, index / gridSize).xyz;*/
}