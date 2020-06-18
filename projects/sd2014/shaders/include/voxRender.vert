// ParticleRender.vert
#version 150 compatibility
#extension GL_EXT_gpu_shader4 : enable

uniform mat4 matProj;
uniform mat4 matView;

uniform sampler2D PosTex;
uniform sampler2D ColorTex;

uniform sampler3D NoiseTex;
uniform float Time;
uniform float CamShake;

varying vec2 CornerVec;

varying vec4 Color;

uniform float Brightness;

void main()
{
	CornerVec = gl_MultiTexCoord0.xy * 2.0 - 1.0;
	ivec2 puv = ivec2(gl_Vertex.xy);
	vec4 posLife = texelFetch(PosTex, puv, 0);
	float noiseTime = Time * .02;
	vec3 pos = posLife.xyz +
		(vec3(texture3D(NoiseTex, vec3(noiseTime, 0, 0)).x, texture3D(NoiseTex, vec3(0, noiseTime, 0)).x, texture3D(NoiseTex, vec3(0, 0, noiseTime)).x) * 2.0 - 1.0) * CamShake;
	gl_Position = Proj * (View * vec4(pos, 1) + vec4(CornerVec * .025, 0, 0));
	if (pos != vec3(0))
	{
		Color = vec4(texelFetch(ColorTex, puv, 0).xyz * Brightness, posLife.w);
	}
	else
	{
		Color = vec4(0);
	}
}
