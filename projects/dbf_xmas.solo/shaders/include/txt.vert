#version 330

uniform mat4 mvm, pm;
uniform mat3 nm;
uniform vec3 offset;
uniform float txtAmount;
uniform float time;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float distance;

out vec3 viewspacePosition3, viewspaceNormal;
out vec4 viewspacePosition4;

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


void main()
{
	vec4 pos = position;
	pos.xyz *= rotx(1.0 - 0.4 - txtAmount);
	pos.xyz *= rotz(1.0 - txtAmount);
	pos.xyz *= roty(1.0 + 0.0 - txtAmount);
	float rv = abs(-1.0 + mod(time*0.5, 2.0))-0.5;
	pos.xyz *= rotz(rv);
	pos.xyz *= txtAmount;
	pos.xyz+=offset;
	//pos.xyz *= 0.9;
	//pos.w *= 2.0;	
	viewspacePosition4 = pos;
	viewspacePosition3 = viewspacePosition4.xyz;
	//viewspaceNormal = normalize(nm * vec3(normal));
	viewspaceNormal = normalize(vec3(normal));

	gl_Position = pm * pos;
}