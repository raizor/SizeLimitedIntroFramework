// ParticleVelPass.frag
#version 150 compatibility
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D OldVelTex;
uniform sampler2D OldPosLifeTex;
uniform sampler3D NoiseTex;
uniform sampler3D NoiseTex2;

uniform float Time;
uniform float TimeStep;

uniform float AccSpeed;
uniform float VelDamp;

float f(vec3 x)
{
	return mix(texture3D(NoiseTex, x).x,texture3D(NoiseTex2, x+.2).x,sin(Time*.8)*.5+.5)*2.-1.;
}

vec3 g(vec3 x)
{
	vec3 y = fract(x);
	return normalize(vec3(
			f(y+vec3(1./256.,0,0))-f(y-vec3(1./256.,0,0)),
			f(y+vec3(0,1./256.,0))-f(y-vec3(0,1./256.,0)),
			f(y+vec3(0,0,1./256.))-f(y-vec3(0,0,1./256.))
		));
}

void main()
{
	ivec2 puv = ivec2(gl_FragCoord.xy);
	gl_FragData[0] = vec4((texelFetch(OldVelTex, puv, 0).xyz * VelDamp) + (g(texelFetch(OldPosLifeTex, puv, 0).xyz * .012) * TimeStep * AccSpeed), 1);

	//gl_FragData[0] = vec4(texelFetch(OldVelTex, puv, 0).xyz, 1.0);
}
