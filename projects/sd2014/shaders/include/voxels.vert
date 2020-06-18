#version 330

uniform mat4 matView;
uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat3 matNormal;
uniform sampler2D texPos;
uniform sampler2D texCol;
uniform sampler3D texNoise;
uniform float time;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

// Color is a per-instance attribute
layout (location = 2) in vec3 posOffset;

// Color is a per-instance attribute
layout (location = 3) in vec4 color;

layout (location = 4) in uint id;

layout (location = 5) in vec2 cornerVec;



out vec4 col;
out vec3 normal;
out vec3 vert;

void main()
{
	float CamShake = 1.0;
	float Brightness = 1.0;
	float noiseTime = time * .02;

	//CornerVec = gl_MultiTexCoord0.xy * 2.0 - 1.0;
	ivec2 puv = ivec2(pos.xy);
	vec4 posLife = texelFetch(texPos, puv, 0);

	vec3 pos = posLife.xyz +
		(vec3(texture(texNoise, vec3(noiseTime, 0, 0)).x, texture(texNoise, vec3(0, noiseTime, 0)).x, texture(texNoise, vec3(0, 0, noiseTime)).x) * 2.0 - 1.0) * CamShake;
	gl_Position = matProjection * (matView * vec4(pos, 1) + vec4(cornerVec * vec2(.025), 0, 0));
	
	if (pos != vec3(0))
	{
		col = vec4(texelFetch(texCol, puv, 0).xyz * Brightness, posLife.w);
	}
	else
	{
		col = vec4(1.0);
	}
	
	/*
	vec3 pos2 = vec3(0.0);//(vec3(texture(texNoise, vec3(noiseTime+id, 0, 0)).x, texture(texNoise, vec3(0, noiseTime+id, 0)).x, texture(texNoise, vec3(0, 0, noiseTime+id)).x) * 2.0 - 1.0) * 100.0;
	
	vec3 newPos = pos + posOffset + posLife.xyz; 	

	col = color;
	vert = vec3(matModelView * vec4(newPos + pos2, 1.0));
	gl_Position = matProjection * vec4(vert, 1.0);
	normal = normalize(matNormal * norm);
	*/
}