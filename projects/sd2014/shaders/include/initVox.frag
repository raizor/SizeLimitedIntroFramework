// ParticlePosInit.frag
#version 150 compatibility
#extension GL_EXT_gpu_shader4 : enable

uniform sampler3D NoiseTex;

float f(vec3 x)
{
	return texture3D(NoiseTex, x).x*2.-1.;
}

vec3 g(vec3 x)
{
	return vec3(
			f(x+vec3(1./256.,0,0))-f(x-vec3(1./256.,0,0)),
			f(x+vec3(0,1./256.,0))-f(x-vec3(0,1./256.,0)),
			f(x+vec3(0,0,1./256.))-f(x-vec3(0,0,1./256.))
		);
}

void main()
{
	vec3 x = g(vec3(gl_FragCoord.xy / vec2(800, 600), 0)) * 20.0f;
	gl_FragData[0] = vec4(x, fract(x.x * 100.0));
}