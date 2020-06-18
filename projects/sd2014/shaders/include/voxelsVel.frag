// add additional, optional velocity texture for directional movement
// seconddary object buffers to store a target position for morpting?

#version 150 compatibility
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D texOldPos;
uniform sampler2D texOldVel;
uniform sampler3D texNoise;
uniform sampler3D texNoise2;

uniform float time;
uniform float timeStep;
uniform float accSpeed;
uniform float damping;

float f(vec3 x)
{
	return mix(texture3D(texNoise, x).x,texture3D(texNoise2, x+.2).x,sin(time*.8)*.5+.5)*2.-1.;
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
	gl_FragData[0] =  vec4(texelFetch(texOldVel, puv, 0).xyz * damping + g(texelFetch(texOldPos, puv, 0).xyz * .012) * (timeStep+0.0001) * accSpeed, 1);
	//gl_FragData[0] = vec4(1, 1, 0, 1);
}